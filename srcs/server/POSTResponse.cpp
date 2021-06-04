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

void POSTResponse::errorExcept()
{
	mResponseContent = ErrorResponse::getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}

void POSTResponse::run()
{
	mBody = getRequestBody();
	checkAuthorization();
	if (getStatusCode())
	{
		errorExcept();
		return ;
	}
	checkTarget();
	if (getStatusCode())
	{
		errorExcept();
		return ;
	}
	try
	{
		writeFile();
		/* 201 created (success) */
		if (getStatusCode() != 201)
		{
			setStatusCode(500);
			errorExcept();
			return ;
		}

		appendResponseHeader();
	}
	catch(std::exception const &e)
	{
		logger::println(TAG, e.what());
		mResponseContent.clear();
		setStatusCode(500);
		errorExcept();
	}
	logger::println(TAG , web::toString(getStatusCode()));
	setState(DONE);
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
	int lastSlashIdx = path.find_last_of("/");

	std::string folder = path.substr(0, lastSlashIdx);
	std::string file = path.substr(lastSlashIdx + 1);

	/* file.empty() : 생성할 file 이름이 없는 경우.. */
	/* isDirectory : folder 경로가 존재하지 않는 경우 404 */
	if (file.empty())
		path += "New File";
	if (!web::isDirectory(folder))
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

void POSTResponse::appendResponseHeader()
{
	mResponseContent += createResponseLine();
	mResponseContent += createDefaultResponseHeader();
	std::string location = mFileName;
	size_t pos = location.find(getLocationConfig()->getRoot());
	location.erase(pos, getLocationConfig()->getRoot().length());

	mResponseContent += "Location: " + location +"\r\n";
	mResponseContent += "Content-Location: " + location +"\r\n";
	mResponseContent += "\r\n";
}
