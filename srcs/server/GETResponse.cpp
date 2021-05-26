#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mState(INDEX_HTML), mContentLocation(""), 
	mResponseContent(NULL)
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
		mResponseContent = new std::string();
		if (mResponseContent)
			*mResponseContent = *rhs.mResponseContent;
	}
	return (*this);
}

GETResponse::~GETResponse()
{

}

std::string *GETResponse::getResponse()
{
	std::string responseBody;
	mResponseContent = new std::string();

	try
	{
		if (mResponseContent)
		{
			setContentLocation();
			responseBody = createResponseBody();

			*mResponseContent += createResponseLine();
			*mResponseContent += createResponseHeader(responseBody);
			*mResponseContent += responseBody;
		}

		if (getStatusCode() != 0)
		{
			delete mResponseContent;
			mResponseContent = NULL;
		}
		else
			setStatusCode(200);
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		delete mResponseContent;
		mResponseContent = NULL;
	}
	return (mResponseContent);
}

std::string GETResponse::createResponseHeader(std::string const & responseBody)
{
	std::string responseHeader;

	/* default header */
	responseHeader += "Date: " + web::getDate() + "\r\n";
	responseHeader += "Server: webserv (chlee, ina)\r\n";
	responseHeader += "Connection: close\r\n";

	/* content part */
	responseHeader += "Content-Language: en-US\r\n";
	responseHeader += "Content-Length: " + web::toString(responseBody.length()) + "\r\n";

	if (mState != AUTOINDEX)
		responseHeader += "Content-Location: " + mContentLocation;

	if (mContentLocation.find_last_of('.') != std::string::npos)
		responseHeader += "Content-Type: " + web::getMIMEType(mContentLocation.substr(mContentLocation.find_last_of('.'))) + "\r\n";
	else
		responseHeader += "Content-Type: text/html\r\n";

	if (mState != AUTOINDEX)
		responseHeader += "Last-Modified: " + web::getDate() + "\r\n";

	responseHeader += "\r\n";
	return (responseHeader);
}

void GETResponse::setContentLocation()
{
	if (mState == INDEX_HTML)
		mContentLocation = getLocationConfig()->getRoot() + getLocationConfig()->getIndexFile();
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
	std::string locationURI = getTarget();
	std::string autoIndexContent;
	struct dirent *file = NULL;
	DIR *dir_ptr = opendir(locationURI.c_str());

	if (dir_ptr == NULL)
		setStatusCode(500);

	autoIndexContent += "<html>";
	autoIndexContent += "<head><title>Index of " + locationURI + "</title></head>";
	autoIndexContent += "<body bgcolor=\"white\">";
	autoIndexContent += "<h1>Index of " + locationURI + "</h1><hr>";
	autoIndexContent += "<pre><a href=\"../\">../</a>";

	while ((file = readdir(dir_ptr)) != NULL)
	{
		fileName = file->d_name;
		autoIndexContent += "<a href=\"" + fileName + "\">" + fileName + "</a>";
		for (size_t idx = 0; idx < 70 - fileName.length(); idx++)
		{
			autoIndexContent += " ";
		}
		autoIndexContent += web::getFileTime();
		autoIndexContent += "<br>";
	}
	autoIndexContent += "</pre><hr></body></html>";
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