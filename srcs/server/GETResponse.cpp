#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mState(INDEX_HTML), mContentLocation("")
{

}

GETResponse::GETResponse(GETResponse const & copy)
: Response(copy)
{
	*this = copy;
}

GETResponse &GETResponse::operator=(GETResponse const & rhs)
{
	if (this != &rhs)
	{
		mContentLocation = rhs.mContentLocation;
		mState = rhs.mState;
	}
	return (*this);
}

GETResponse::~GETResponse()
{

}

std::string *GETResponse::getResponse()
{
	initState();
	if (getStatusCode() != 0)
		return (NULL);
	std::string responseBody;
	std::string *responseContent;
	try
	{
		responseContent = new std::string();
		if (responseContent)
		{
			initContentLocation();
			responseBody = createResponseBody();
			if (getStatusCode() == 0)
				setStatusCode(200);
			*responseContent += createResponseLine();
			createResponseHeader(responseBody, *responseContent);
			*responseContent += responseBody;
		}
		if (getStatusCode() != 200)
		{
			delete responseContent;
			responseContent = NULL;
		}
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

void GETResponse::createResponseHeader(std::string const & responseBody, std::string & responseContent)
{
	/* default header */
	responseContent += "Date: " + web::getDate() + "\r\n";
	responseContent += "Server: webserv (chlee, ina)\r\n";
	responseContent += "Connection: close\r\n";

	/* content part */
	responseContent += "Content-Language: en-US\r\n";
	responseContent += "Content-Length: " + web::toString(responseBody.length()) + "\r\n";

	if (mState != AUTOINDEX)
		responseContent += "Content-Location: " + mContentLocation;

	if (mContentLocation.find_last_of('.') != std::string::npos)
		responseContent += "Content-Type: " + web::getMIMEType(mContentLocation.substr(mContentLocation.find_last_of('.'))) + "\r\n";
	else if (mState == AUTOINDEX)
		responseContent += "Content-Type: text/html\r\n";
	else
		responseContent += "Content-Type: text/plain\r\n";

	if (mState != AUTOINDEX)
		responseContent += "Last-Modified: " + web::getLastModifiedTime(mContentLocation) + "\r\n";

	responseContent += "\r\n";
}

void GETResponse::initState()
{
	std::string contentLocation = getLocationConfig()->getRoot() + getTargetContent();

	contentLocation = web::removeConsecutiveDuplicate(contentLocation, '/');
	if (isDirectory(contentLocation.c_str()))
	{
		/* Test GET Expected 404 on http://localhost:8080/directory/Yeah
			FATAL ERROR ON LAST TEST: bad status code */
		/* 이 테스트를 어떻게 처리할까 생각하다 autoindex, index의 관계를 생각했고.. 
			아래처럼 수정했습니다. 참고해주세요.*/
		if (contentLocation[contentLocation.length() - 1] != '/')
			contentLocation += "/";
		std::string indexPath = contentLocation + getLocationConfig()->getIndexFile();
		logger::print(TAG) << "indexPath: " << indexPath << std::endl;
		if (web::isPathExist(indexPath.c_str()))
			mState = INDEX_HTML;
		else if (getLocationConfig()->isAutoIndex())
			mState = AUTOINDEX;
		else
			setStatusCode(404);
	}
	else if (web::isPathExist(contentLocation.c_str()))
			mState = TARGET;
	else
		setStatusCode(404);
}

void GETResponse::initContentLocation()
{
	if (mState == INDEX_HTML)
		mContentLocation = getLocationConfig()->getRoot() + getTargetContent() + "/" + getLocationConfig()->getIndexFile();
	else if (mState == TARGET)
		mContentLocation = getLocationConfig()->getRoot() + getTargetContent();
	else if (mState == AUTOINDEX)
		mContentLocation = getLocationConfig()->getRoot() + getTargetContent() + "/";
	mContentLocation = web::removeConsecutiveDuplicate(mContentLocation, '/');
}

std::string GETResponse::createResponseBody()
{
	if (mState == AUTOINDEX)
		return (makeAutoIndexContent() + "\r\n");
	return (readContentLocation() + "\r\n");
}

std::string GETResponse::makeAutoIndexContent()
{
	std::string fileName;
	std::string filePath;
	std::string autoIndexContent;
	std::map<std::string, std::string> requestHeader = getRequestHeader();
	struct dirent *file = NULL;
	DIR *directoryPointer = opendir(mContentLocation.c_str());

	if (directoryPointer == NULL)
		setStatusCode(500);

	autoIndexContent += "<html>";
	autoIndexContent += "<head><title>Index of " + getTarget() + "</title></head>";
	autoIndexContent += "<body bgcolor=\"white\">";
	autoIndexContent += "<h1>Index of " + getTarget() + "</h1><hr>";
	autoIndexContent += "<pre>";

	while ((file = readdir(directoryPointer)) != NULL)
	{
		fileName = file->d_name;
		filePath = "http://" + requestHeader["HOST"] + web::removeConsecutiveDuplicate(getTarget() + '/' + fileName, '/');

		autoIndexContent += "<a href=\"" + filePath + "\">" + fileName + "</a>";
		for (size_t idx = 0; idx < 70 - fileName.length(); idx++)
		{
			autoIndexContent += " ";
		}
		if (fileName != "." && fileName != "..")
			autoIndexContent += web::getFileTime(mContentLocation + fileName);
		autoIndexContent += "<br>";
	}
	autoIndexContent += "</pre><hr></body></html>";
	closedir(directoryPointer);
	return (autoIndexContent);
}

std::string GETResponse::readContentLocation()
{
	File file(mContentLocation);
	std::string content = "";

	try
	{
		file.openFile();
		while (file.getState() == File::CONTENT_LEFT)
		{
			file.readFile();
		}
		content = file.getBuffer();
		file.closeFile();
	}
	catch(const std::exception& e)
	{
		setStatusCode(404);
		logger::println(TAG, e.what());
	}
	return (content);
}

bool GETResponse::isDirectory(const char *target)
{
	struct stat buf;
	if (stat(target, &buf) < 0)
		setStatusCode(500);

	if (S_ISDIR(buf.st_mode))
		return (true);
	else
		return (false);
}
