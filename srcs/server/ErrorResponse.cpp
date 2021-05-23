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
		getServerManager().addFD(mFile.getFD(), mFDListener);
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
		responseHeader += "Content-Length: " + web::toString(std::string("<html><head></head><body>asdf</body></html>\r\n").size()) + "\r\n";
	else
		responseHeader += "Content-Length: " + web::toString(mFile.getBuffer().size() + 2) + "\r\n";
	// getStatusCode() + getStatusMessage(status code);
	// TODO Header에 넣어야 하는 field들 확인 (Content Length같은 경우는 string.size() + 2로 할 예정 등...)
	responseHeader += "\r\n";
	logger::println(TAG, responseHeader);
	return (responseHeader);
}

std::string ErrorResponse::createResponseBody()
{
	if (mIsDefault)
	{
		// return static default error
	}
	else
		return (mFile.getBuffer() + "\r\n");
	return ("<html><head></head><body>asdf</body></html>\r\n");
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

