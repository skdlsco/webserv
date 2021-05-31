#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mState(INDEX_HTML), mContentLocation("")
{
	int fd;

	if (isDirectory(getTarget().c_str()))
	{
		if (getLocationConfig()->isAutoIndex())
			mState = AUTOINDEX;
		else
			mState = INDEX_HTML;
	}
	else
	{
		if ((fd = open(getTarget().c_str(), O_RDONLY)) < 0)
			mState = INDEX_HTML;
		else
		{
			mState = TARGET;
			close(fd);
		}
	}
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
	std::string responseBody;
	std::string *responseContent;
	try
	{
		responseContent = new std::string();
		if (responseContent)
		{
			setContentLocation();
			responseBody = createResponseBody();

			*responseContent += createResponseLine();
			createResponseHeader(responseBody, *responseContent);
			*responseContent += responseBody;
		}

		if (getStatusCode() != 0)
		{
			delete responseContent;
			responseContent = NULL;
		}
		else
			setStatusCode(200);
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
	else
		responseContent += "Content-Type: text/plain\r\n";

	if (mState != AUTOINDEX)
		responseContent += "Last-Modified: " + web::getLastModifiedTime(mContentLocation) + "\r\n";

	responseContent += "\r\n";
}

void GETResponse::setContentLocation()
{
	if (mState == INDEX_HTML)
	{
		mContentLocation = getLocationConfig()->getRoot() + getLocationConfig()->getIndexFile();
		mContentLocation = web::removeConsecutiveDuplicate(mContentLocation, '/');
	}
	else if (mState == TARGET)
		mContentLocation = getTarget();
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
	std::string locationURI = getTarget();
	std::string autoIndexContent;
	struct dirent *file = NULL;
	DIR *directoryPointer = opendir(locationURI.c_str());

	if (directoryPointer == NULL)
		setStatusCode(500);

	autoIndexContent += "<html>";
	autoIndexContent += "<head><title>Index of " + locationURI + "</title></head>";
	autoIndexContent += "<body bgcolor=\"white\">";
	autoIndexContent += "<h1>Index of " + locationURI + "</h1><hr>";
	autoIndexContent += "<pre><a href=\"../\">../</a>";

	while ((file = readdir(directoryPointer)) != NULL)
	{
		fileName = file->d_name;
		filePath = locationURI + fileName;

		if (file->d_name != "." && file->d_name != "..")
			autoIndexContent += "<a href=\"" + filePath + "\">" + fileName + "</a>";
		else
			autoIndexContent += "<a href=\"" + fileName + "\">" + fileName + "</a>";
		for (size_t idx = 0; idx < 70 - fileName.length(); idx++)
		{
			autoIndexContent += " ";
		}
		if (file->d_name != "." && file->d_name != "..")
			autoIndexContent += web::getFileTime(filePath);
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