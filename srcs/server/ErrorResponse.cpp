#include "ErrorResponse.hpp"

std::string const ErrorResponse::TAG = "ErrorResponse";

ErrorResponse::ErrorResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
								const LocationConfig * locationConfig)
: Response(serverManager, serverConfig, locationConfig), mIsDefault(false),
	mFile(getServerConfig()->getDefaultErrorPagePath()), mFDListener(*this)
{
	try
	{
		mFile.openFile();
		getServerManager(). addFD(mFile.getFD(), mFDListener);
	}
	catch(const std::exception& e)
	{
		mIsDefault = true;
		setState(Response::DONE);
	}
}

ErrorResponse::~ErrorResponse()
{
	setErrorToDefault();
}

void ErrorResponse::setErrorToDefault()
{
	if (!mIsDefault)
	{
		mIsDefault = true;
		getServerManager().removeFD(mFile.getFD());
		mFile.closeFile();
		setState(Response::DONE);
	}
}

std::string ErrorResponse::createResponseHeader()
{
	std::string responseHeader;

	responseHeader += "Date: " + web::getDate() + "\r\n";
	responseHeader += "Server: webserv (chlee, ina)\r\n";
	responseHeader += "Connection: close\r\n";
	responseHeader += "Content-Type: text/html\r\n";

	if (mIsDefault)
		responseHeader += "Content-Length: " + web::toString(web::getErrorPage(getStatusCode()).size()) + "\r\n";
	else
		responseHeader += "Content-Length: " + web::toString(mFile.getBuffer().size() + 2) + "\r\n";
	responseHeader += "\r\n";
	return (responseHeader);
}

std::string ErrorResponse::createResponseBody()
{
	if (mIsDefault)
	{
		return (web::getErrorPage(getStatusCode()));
	}
	return (mFile.getBuffer() + "\r\n");
}

ErrorResponse::ErrorResponseFDListener::ErrorResponseFDListener(ErrorResponse &errorResponse)
: mErrorResponse(errorResponse)
{

}

ErrorResponse::ErrorResponseFDListener::~ErrorResponseFDListener()
{

}

void ErrorResponse::ErrorResponseFDListener::onReadSet()
{
	if (mErrorResponse.mIsDefault || mErrorResponse.getState() != ON_WORKING)
		return ;
	try
	{
		mErrorResponse.mFile.readFile();
		if (mErrorResponse.mFile.getState() != File::CONTENT_LEFT)
			mErrorResponse.setState(Response::DONE);
	}
	catch(const std::exception& e)
	{
		mErrorResponse.setErrorToDefault();
	}
}

void ErrorResponse::ErrorResponseFDListener::onWriteSet()
{

}

void ErrorResponse::ErrorResponseFDListener::onExceptSet()
{
	mErrorResponse.setErrorToDefault();
}

