#include "HEADResponse.hpp"

std::string const HEADResponse::TAG = "HEADResponse";

HEADResponse::HEADResponse(const ServerConfig * serverConfig,
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

HEADResponse::HEADResponse(HEADResponse const & copy)
: Response(copy)
{
	*this = copy;
}

HEADResponse &HEADResponse::operator=(HEADResponse const & rhs)
{
	if (this != &rhs)
	{
		mContentLocation = rhs.mContentLocation;
		mState = rhs.mState;

		delete mResponseContent;
		mResponseContent = new std::string();
		*mResponseContent = *rhs.mResponseContent;
	}
	return (*this);
}

HEADResponse::~HEADResponse()
{
	if (mResponseContent)
		delete mResponseContent;
}

std::string *HEADResponse::getResponse()
{
	std::string responseBody;

	try
	{
		mResponseContent = new std::string();
		if (mResponseContent)
		{
			setContentLocation();
			responseBody = createResponseBody();

			*mResponseContent += createResponseLine();
			createResponseHeader(responseBody);

			/* HEAD Method Response don't have body content */
			/* *mResponseContent += createResponseBody(); */
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

void HEADResponse::createResponseHeader(std::string const & responseBody)
{
	/* default header */
	*mResponseContent += "Date: " + web::getDate() + "\r\n";
	*mResponseContent += "Server: webserv (chlee, ina)\r\n";
	*mResponseContent += "Connection: close\r\n";

	/* content part */
	*mResponseContent += "Content-Language: en-US\r\n";
	*mResponseContent += "Content-Length: " + web::toString(responseBody.length()) + "\r\n";

	if (mState != AUTOINDEX)
		*mResponseContent += "Content-Location: " + mContentLocation;

	if (mContentLocation.find_last_of('.') != std::string::npos)
		*mResponseContent += "Content-Type: " + web::getMIMEType(mContentLocation.substr(mContentLocation.find_last_of('.'))) + "\r\n";
	else
		*mResponseContent += "Content-Type: text/html\r\n";

	if (mState != AUTOINDEX)
		*mResponseContent += "Last-Modified: " + web::getDate() + "\r\n";

	*mResponseContent += "\r\n";
}

void HEADResponse::setContentLocation()
{
	if (mState == INDEX_HTML)
		mContentLocation = getLocationConfig()->getRoot() + getLocationConfig()->getIndexFile();
	else if (mState == TARGET)
		mContentLocation = getTarget();
}

std::string HEADResponse::createResponseBody()
{
	if (mState == AUTOINDEX)
		return (makeAutoIndexContent() + "\r\n");
	return (readContentLocation() + "\r\n");
}

std::string HEADResponse::makeAutoIndexContent()
{
	std::string fileName;
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

std::string HEADResponse::readContentLocation()
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

bool HEADResponse::isDirectory(const char *target)
{
	struct stat buf;
	if (stat(target, &buf) < 0)
		setStatusCode(500);

	if (S_ISDIR(buf.st_mode))
		return (true);
	else
		return (false);
}
