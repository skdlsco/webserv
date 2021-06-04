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

void DELETEResponse::errorExcept()
{
	mResponseContent = ErrorResponse::getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}

void DELETEResponse::run()
{
	checkTarget();
	if (getStatusCode())
	{
		errorExcept();
		return ;
	}
	try
	{
		deleteFile();
		if (getStatusCode() != 204)
		{
			errorExcept();
			return ;
		}
		appendResponseHeader();
		appendResponseBody();
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		errorExcept();
	}
}

void DELETEResponse::checkTarget()
{
	std::string path = getLocationConfig()->getRoot() + getTarget();

	path = web::removeConsecutiveDuplicate(path, '/');
	if (!web::isPathExist(path) || web::isDirectory(path))
	{
		setStatusCode(404);
		return ;
	}
	mDeleteTarget = path;
}

void DELETEResponse::appendResponseHeader()
{
	mResponseContent += createResponseLine();
	mResponseContent += createDefaultResponseHeader();
	mResponseContent += "Content-Language: en-US\r\n";
	mResponseContent += "Content-Length: 0\r\n";
	mResponseContent += "\r\n";
}

void DELETEResponse::appendResponseBody()
{
	mResponseContent += "\r\n";
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