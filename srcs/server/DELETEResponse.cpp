#include "DELETEResponse.hpp"

std::string const DELETEResponse::TAG = "DELETEResponse";

DELETEResponse::DELETEResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mDeleteTarget("")
{

}

DELETEResponse::~DELETEResponse()
{

}

DELETEResponse::DELETEResponse(DELETEResponse const & copy)
: Response(copy)
{
	*this = copy;
}

DELETEResponse &DELETEResponse::operator=(DELETEResponse const & rhs)
{
	Response::operator=(rhs);
	return (*this);
}

std::string *DELETEResponse::getResponse()
{
	std::string *responseContent = NULL;

	checkTarget();
	if (getStatusCode())
		return (NULL);
	try
	{
		deleteFile();
		if (getStatusCode() != 204)
			return (NULL);
		*responseContent += createResponseLine();
		appendResponseHeader(*responseContent);
		appendResponseBody(*responseContent);
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		delete responseContent;
		responseContent = NULL;
	}
	return (responseContent);
}

void DELETEResponse::checkTarget()
{
	std::string path = getLocationConfig()->getRoot() + getTarget();

	path = web::removeConsecutiveDuplicate(path, '/');

	if (!web::isFilePath(path) || !web::isFileExist(path))
	{
		/* 폴더인 경우에도 404를 띄우는게 맞을까요? */
		setStatusCode(404);
		return ;
	}
	else if (!web::isFileExist(path))
	{
		setStatusCode(404);
		return ;
	}
	mDeleteTarget = path;
}

void DELETEResponse::appendResponseHeader(std::string &responseContent)
{
	std::string responseHeader;

	responseContent += "Date: " + web::getDate() + "\r\n";
	responseContent += "Server: webserv (chlee, ina)\r\n";
	responseContent += "Connection: close\r\n";

	responseContent += "Content-Language: en-US\r\n";
	responseContent += "Content-Length: 0\r\n";
	responseContent += "\r\n";
}

void DELETEResponse::appendResponseBody(std::string &responseContent)
{
	responseContent += "\r\n";
}

void DELETEResponse::deleteFile()
{
	if (unlink(mDeleteTarget.c_str()) == -1)
	{
		/* access error - forbidden */
		setStatusCode(403);
		return ;
	}
	/* no content */
	setStatusCode(204);
}