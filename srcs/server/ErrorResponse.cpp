#include "ErrorResponse.hpp"

std::string const ErrorResponse::TAG = "ErrorResponse";

ErrorResponse::ErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mIsDefault(false),
	mFile(web::removeConsecutiveDuplicate(getServerConfig()->getRoot() + getServerConfig()->getDefaultErrorPagePath(), '/'))
{

}

ErrorResponse::~ErrorResponse()
{
	
}

ErrorResponse::ErrorResponse(ErrorResponse const & copy)
: Response(copy), mFile(copy.mFile)
{
	*this = copy;
}

ErrorResponse &ErrorResponse::operator=(ErrorResponse const & rhs)
{
	if (this != &rhs)
	{
		this->mFile = rhs.mFile;
		this->mIsDefault = rhs.mIsDefault;
	}
	return (*this);
}

void ErrorResponse::run()
{
	mResponseContent = getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}

std::string ErrorResponse::getErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig, int errorCode)
{
	ErrorResponse errorResponse(serverConfig, locationConfig);
	errorResponse.setStatusCode(errorCode);
	return (errorResponse.createResponse());
}

std::string ErrorResponse::createResponse()
{
	createUserErrorPage();
	try
	{
		appendResponseHeader();
		appendResponseBody();
	}
	catch(std::exception const&e)
	{
		logger::println(TAG, e.what());
	}
	return (mResponseContent);
}

void ErrorResponse::createUserErrorPage()
{
	try
	{
		mFile.openFile();
		while (mFile.getState() == File::CONTENT_LEFT)
		{
			mFile.readFile();
		}
	}
	catch(std::exception const &e)
	{
		setErrorToDefault();
		logger::println(TAG, e.what());
	}
}

void ErrorResponse::setErrorToDefault()
{
	if (!mIsDefault)
	{
		mIsDefault = true;
		mFile.closeFile();
	}
}

std::string ErrorResponse::getAllowMethod()
{
	if (getLocationConfig() == NULL)
		return ("");

	std::vector<std::string> methodVector = getLocationConfig()->getAllowMethodList();
	std::string result;

	for (std::vector<std::string>::iterator iter = methodVector.begin(); iter != methodVector.end(); iter++)
	{
		result += *iter;
		result += " ";
	}
	return (result);
}

void ErrorResponse::appendResponseHeader()
{
	mResponseContent += createResponseLine();
	mResponseContent += createDefaultResponseHeader();

	mResponseContent += "Content-Type: text/html\r\n";

	/* 503 timeout */
	if (getStatusCode() == 503)
		mResponseContent += "Retry-After: 120\r\n";
	/* 405 not allowed method */
	if (getStatusCode() == 405)
		mResponseContent += "Allow: " + getAllowMethod() + "\r\n";
	/* 401 unauthorized */
	if (getStatusCode() == 401)
		mResponseContent += "WWW-Authenticate: Basic realm=\"webserv\"\r\n";

	if (mIsDefault)
		mResponseContent += "Content-Length: " + web::toString(web::getErrorPage(getStatusCode()).size()) + "\r\n";
	else
		mResponseContent += "Content-Length: " + web::toString(mFile.getBuffer().size()) + "\r\n";
	mResponseContent += "\r\n";
}

void ErrorResponse::appendResponseBody()
{
	if (mIsDefault)
		mResponseContent += web::getErrorPage(getStatusCode());
	else
		mResponseContent += mFile.getBuffer();
}
