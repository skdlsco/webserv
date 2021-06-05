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


void PUTResponse::errorExcept()
{
	mResponseContent = ErrorResponse::getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}

void PUTResponse::run()
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
		/* 204 no content, 201 created (success) */
		if (getStatusCode() / 100 != 2)
		{
			setStatusCode(500);
			errorExcept();
			return ;
		}

		appendResponseHeader();
		if (getStatusCode() == 201)
			appendResponseBody();
	}
	catch(std::exception const &e)
	{
		logger::println(TAG, e.what());
		mResponseContent.clear();
		setStatusCode(500);
		errorExcept();
	}
	setState(DONE);
}

void PUTResponse::checkAuthorization()
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

void PUTResponse::checkTarget()
{
	std::string path = getLocationConfig()->getRoot() + getTargetContent();

	path = web::removeConsecutiveDuplicate(path, '/');

	if (path[path.length() - 1] == '/')
		path.erase(path.length() - 1);
	int lastSlashIdx = path.find_last_of("/");
	std::string folder = path.substr(0, lastSlashIdx);
	std::string file = path.substr(lastSlashIdx + 1);

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
	else if (isExist)
		setStatusCode(204);
	else
		setStatusCode(201);
}

void PUTResponse::appendResponseHeader()
{
	mResponseContent += createResponseLine();
	mResponseContent += createDefaultResponseHeader();

	std::string location = mFileName;
	size_t pos = location.find(getLocationConfig()->getRoot());
	location.erase(pos, getLocationConfig()->getRoot().length());

	if (getStatusCode() == 201)
		mResponseContent += "Content-Length: " + web::toString(mBody.size()) + "\r\n";
	mResponseContent += "Content-Location: " + location + "\r\n";
	mResponseContent += "\r\n";
}

void PUTResponse::appendResponseBody()
{
	mResponseContent += mBody;
}
