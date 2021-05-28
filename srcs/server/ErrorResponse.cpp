#include "ErrorResponse.hpp"

std::string const ErrorResponse::TAG = "ErrorResponse";

ErrorResponse::ErrorResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mIsDefault(false),
	mFile(getServerConfig()->getDefaultErrorPagePath())
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

std::string *ErrorResponse::getResponse()
{
	std::string *responseContent;

	createUserErrorPage();
	try
	{
		responseContent = new std::string();
		if (responseContent)
		{
			*responseContent += createResponseLine();
			createResponseHeader(responseContent);
			createResponseBody(responseContent);
		}
	}
	catch(std::exception const&e)
	{
		logger::println(TAG, e.what());
		delete responseContent;
		responseContent = NULL;
	}
	return (responseContent);
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

void ErrorResponse::createResponseHeader(std::string *responseContent)
{
	if (responseContent == NULL)
		return ;
	*responseContent += "Date: " + web::getDate() + "\r\n";
	*responseContent += "Server: webserv (chlee, ina)\r\n";
	*responseContent += "Connection: close\r\n";
	*responseContent += "Content-Type: text/html\r\n";

	/* 503 timeout */
	if (getStatusCode() == 503)
		*responseContent += "Retry-After: 120\r\n";
	/* 405 not allowed method */
	if (getStatusCode() == 405)
		*responseContent += "Allow: " + getAllowMethod() + "\r\n";
	/* 401 unauthorized */
	if (getStatusCode() == 401)
		*responseContent += "WWW-Authenticate: Basic realm=\"webserv\"\r\n";

	if (mIsDefault)
		*responseContent += "Content-Length: " + web::toString(web::getErrorPage(getStatusCode()).size()) + "\r\n";
	else
		*responseContent += "Content-Length: " + web::toString(mFile.getBuffer().size()) + "\r\n";
	*responseContent += "\r\n";
}

void ErrorResponse::createResponseBody(std::string *responseContent)
{
	if (responseContent == NULL)
		return ;
	if (mIsDefault)
		*responseContent += web::getErrorPage(getStatusCode());
	else
		*responseContent += mFile.getBuffer();
}
