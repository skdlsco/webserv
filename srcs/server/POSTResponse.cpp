#include "POSTResponse.hpp"

std::string const POSTResponse::TAG = "POST Response";

POSTResponse::POSTResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig)
{

}

POSTResponse::~POSTResponse()
{

}

POSTResponse::POSTResponse(POSTResponse const & copy)
: Response(copy)
{
	*this = copy;
}

POSTResponse &POSTResponse::operator=(POSTResponse const & rhs)
{
	Response::operator=(rhs);
	return (*this);
}

std::string *POSTResponse::getResponse()
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
		/* 201 created (success) */
		if (getStatusCode() != 201)
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

void POSTResponse::checkAuthorization()
{
	if (getLocationConfig()->getAuthUserName().empty() || getLocationConfig()->getAuthUserPassword().empty())
		return ;
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

void POSTResponse::createFileName(std::string const & path)
{
	if (!web::isPathExist(path))
	{
		mFileName = path;
		return ;
	}
	int number = 1;
	while (number > 0)
	{
		std::string fileName = path + " (" + web::toString(number) + ")";
		if (!web::isPathExist(fileName))
		{
			mFileName = fileName;
			return ;
		}
		number++;
	}
}

void POSTResponse::checkTarget()
{
	std::string path = getLocationConfig()->getRoot() + getTargetContent();

	path = web::removeConsecutiveDuplicate(path, '/');
	int slashIdx = path.find_last_of("/");

	std::string folder = path.substr(0, slashIdx);
	std::string file = path.substr(slashIdx + 1);

	/* POST가 원래 file을 그냥 생성하는 것이었는데
	   target이 항상 폴더여야 하고, 이름을 내부에서 정하는 것으로 하는게 맞는 것 같아서 고민이 되네요 */
	/* file.empty() : 생성할 file 이름이 없는 경우.. */
	/* isDirectory : folder 경로가 존재하지 않는 경우 404 */
	if (file.empty() || !web::isDirectory(folder))
	{
		setStatusCode(404);
		return ;
	}
	createFileName(path);
}

void POSTResponse::writeFile()
{
	int fd = open(mFileName.c_str(), O_CREAT | O_WRONLY, 0777);
	if (fd == -1)
	{
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
	 else
		setStatusCode(201);
}

void POSTResponse::appendResponseHeader(std::string &responseContent)
{
	std::string responseHeader;

	responseContent += "Date: " + web::getDate() + "\r\n";
	responseContent += "Server: webserv (chlee, ina)\r\n";
	responseContent += "Connection: close\r\n";

	std::string location = mFileName;
	size_t pos = location.find(getLocationConfig()->getRoot());
	location.erase(pos, getLocationConfig()->getRoot().length());

	responseContent += "Location: " + location +"\r\n";
	responseContent += "Content-Location: " + location +"\r\n";
	responseContent += "\r\n";
}

void POSTResponse::appendResponseBody(std::string &responseContent)
{
	responseContent += "\r\n";
}
