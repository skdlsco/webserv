#include "PUTResponse.hpp"

std::string const PUTResponse::TAG = "PUT Response";

PUTResponse::PUTResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig)
{

}

PUTResponse::~PUTResponse()
{

}

PUTResponse::PUTResponse(PUTResponse const & copy)
: Response(copy)
{
	*this = copy;
}

PUTResponse &PUTResponse::operator=(PUTResponse const & rhs)
{
	Response::operator=(rhs);
	return (*this);
}

std::string *PUTResponse::getResponse()
{
	std::string *responseContent = NULL;

	mBody = getRequestBody();
	checkAuthorization();
	if (getStatusCode())
		return (NULL);
	checkTarget();
	if (getStatusCode())
		return (NULL);
	try
	{
		writeFile();
		/* 204 no content, 201 created (success) */
		if (getStatusCode() / 100 != 2)
			return (NULL);
		responseContent = new std::string();
		if (responseContent)
		{
			*responseContent += createResponseLine();
			appendResponseHeader(*responseContent);
			appendResponseBody(*responseContent);
		}
	}
	catch(std::exception const &e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		delete responseContent;
		responseContent = NULL;
	}
	return (responseContent);
}

void PUTResponse::checkAuthorization()
{
	if (getLocationConfig()->getAuthUserName().empty() || getLocationConfig()->getAuthUserPassword().empty())
		return ;
	logger::print(TAG) << "this PUT Request have AuthInfo";
	std::map<std::string, std::string> requestHeader = getRequestHeader();
	if (requestHeader.find("Authorization") == requestHeader.end())
	{
		/* Unauthorized */
		setStatusCode(401);
		return ;
	}
	std::string userAuth = web::base64Decoder(requestHeader["Authorization"]);
	std::string serverAuth = getLocationConfig()->getAuthUserName() + ":" + getLocationConfig()->getAuthUserPassword();
	if (userAuth != serverAuth)
	{
		/* Forbidden */
		setStatusCode(403);
		return ;
	}
}

void PUTResponse::checkTarget()
{
	std::string path = getLocationConfig()->getRoot() + getTargetContent();

	path = web::removeConsecutiveDuplicate(path, '/');
<<<<<<< HEAD
	
	logger::print(TAG) << "path: " << path << std::endl;
=======

	if (path[path.length() - 1] == '/')
		path.erase(path.length() - 1);
>>>>>>> 0fa402c68b89dfa72ab27a9eca27b631ba7e2dc6
	int slashIdx = path.find_last_of("/");
	std::string folder = path.substr(0, slashIdx);
	std::string file = path.substr(slashIdx + 1);

	/* file.empty() : PUT할 file에 대한 input이 없는경우 */
	/* isDirectory : folder 경로가 존재하지 않는 경우 404 */
	if (file.empty() || !web::isDirectory(folder))
	{
		setStatusCode(404);
		return ;
	}
	mFileName = path;
}

void PUTResponse::writeFile()
{
	bool isExist = web::isPathExist(mFileName);

	//to log
	if (isExist)
		logger::println(TAG, "isFileExist");

	logger::println(TAG, "mFilename: "  + mFileName);
	int fd = open(mFileName.c_str(), O_CREAT | O_WRONLY);
	if (fd == -1)
	{
		logger::println(TAG, "file not open");
		setStatusCode(500);
		return ;
	}
	int writeN;
	size_t bufferSize = BUFFER_SIZE;

	if (BUFFER_SIZE > mBody.length())
		bufferSize = mBody.length();
	while ((writeN = write(fd, mBody.c_str(), bufferSize)) > 0)
	{
		mBody.erase(0, writeN);
		if (BUFFER_SIZE > mBody.length())
			bufferSize = mBody.length();
	}
	close(fd);
	if (writeN == -1)
		setStatusCode(500);
	else if (isExist)
		setStatusCode(204);
	else
		setStatusCode(201);
}

void PUTResponse::appendResponseHeader(std::string &responseContent)
{
	std::string responseHeader;

	responseContent += "Date: " + web::getDate() + "\r\n";
	responseContent += "Server: webserv (chlee, ina)\r\n";
	responseContent += "Connection: close\r\n";

	std::string location = mFileName;
	size_t pos = location.find(getLocationConfig()->getRoot());
	location.erase(pos, getLocationConfig()->getRoot().length());

	responseContent += "Content-Location: " + location +"\r\n";
	responseContent += "\r\n";
}

void PUTResponse::appendResponseBody(std::string &responseContent)
{
	responseContent += "\r\n";
}
