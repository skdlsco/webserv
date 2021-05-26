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

std::string *ErrorResponse::getResponse()
{
	createUserErrorPage();
	mResponseContent = new std::string();
	try
	{
		if (mResponseContent)
		{
			*mResponseContent += createResponseLine();
			createResponseHeader();
			createResponseBody();
		}
	}
	catch(std::exception const&e)
	{
		logger::println(TAG, e.what());
		delete mResponseContent;
		mResponseContent = NULL;
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

void ErrorResponse::createResponseHeader()
{
	if (mResponseContent == NULL)
		return ;
	*mResponseContent += "Date: " + web::getDate() + "\r\n";
	*mResponseContent += "Server: webserv (chlee, ina)\r\n";
	*mResponseContent += "Connection: close\r\n";
	*mResponseContent += "Content-Type: text/html\r\n";

	/* 503 timeout */
	if (getStatusCode() == 503)
		*mResponseContent += "Retry-After: 120\r\n";
	/* 405 not allowed method */
	if (getStatusCode() == 405)
		*mResponseContent += "Allow: " + getAllowMethod() + "\r\n";
	/* 401 unauthorized */
	if (getStatusCode() == 401)
		*mResponseContent += "WWW-Authenticate: Basic realm=\"webserv\"\r\n";

	if (mIsDefault)
		*mResponseContent += "Content-Length: " + web::toString(web::getErrorPage(getStatusCode()).size()) + "\r\n";
	else
		*mResponseContent += "Content-Length: " + web::toString(mFile.getBuffer().size() + 2) + "\r\n";
	*mResponseContent += "\r\n";
}

void ErrorResponse::createResponseBody()
{
	if (mResponseContent == NULL)
		return ;
	if (mIsDefault)
		*mResponseContent += web::getErrorPage(getStatusCode());
	else
		*mResponseContent += mFile.getBuffer();
}
