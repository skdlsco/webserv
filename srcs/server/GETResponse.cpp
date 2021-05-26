#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: Response(serverManager, serverConfig, locationConfig), mContentLocation(""), 
	mBody(""), mState(INDEX_HTML)
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
		mBody = rhs.mBody;
		mState = rhs.mState;
	}
	return (*this);
}

GETResponse::~GETResponse()
{

}

void GETResponse::run()
{
	int fd;

	if (isDirectory(getTarget().c_str()))
	{
		/* directory & autoindex check*/
		if (getLocationConfig()->isAutoIndex())
			mState = AUTOINDEX;
		else
			mState = INDEX_HTML;
	}
	else
	{
		/* file */
		if ((fd = open(getTarget().c_str(), O_RDONLY)) < 0)
			mState = INDEX_HTML;
		else
		{
			mState = TARGET;
			close(fd);
		}
	}

	/* execute once to error check. but... not good */
	// mResponse = new std::string(createResponseLine() + createResponseBody() + createResponseHeader());

	/* to connection call getResponse() */
	// if (getState() != ERROR)
	// 	setState(DONE);
}

std::string *GETResponse::getResponse()
{
	if (getState() == ERROR)
		return (NULL);
	else
		return (new std::string(createResponseLine() + createResponseHeader() + createResponseBody()));
}

std::string GETResponse::createResponseHeader()
{
	std::string responseHeader;
	std::string extension;

	/* default header */
	responseHeader += "Date: " + web::getDate() + "\r\n";
	responseHeader += "Server: webserv (chlee, ina)\r\n";
	responseHeader += "Connection: close\r\n";

	/* content part */
	responseHeader += "Content-Language: en-US\r\n";
	responseHeader += "Content-Length: " + web::toString(mBody.length() + 2) + "\r\n";

	if (mState != AUTOINDEX)
		responseHeader += "Content-Location: " + mContentLocation;

	if (mContentLocation.find_last_of('.') != std::string::npos)
	{
		extension = mContentLocation.substr(mContentLocation.find_last_of('.'));
		responseHeader += "Content-Type: " + web::getMIMEType(extension) + "\r\n";
	}
	else
		responseHeader += "Content-Type: text/html\r\n";

	if (mState != AUTOINDEX)
		responseHeader += "Last-Modified: " + web::getDate() + "\r\n";

	responseHeader += "\r\n";
	return (responseHeader);
}

std::string GETResponse::createResponseBody()
{
	std::string responseBody = "";

	if (mState == INDEX_HTML)
	{
		mContentLocation = getLocationConfig()->getRoot() + getLocationConfig()->getIndexFile();
		responseBody = readIndexPageContent() + "\r\n";
	}
	else if (mState == AUTOINDEX)
	{
		responseBody = makeAutoIndexContent() + "\r\n";
	}
	else if (mState == TARGET)
	{
		mContentLocation = getTarget();
		responseBody = readTargetContent() + "\r\n";
	}
	logger::println(TAG, "mContentLocation : " + mContentLocation);
	return (responseBody);
}

std::string GETResponse::makeAutoIndexContent()
{
	std::string fileName;
	std::string locationURI = getTarget();
	std::string autoIndexContent;
	DIR *dir_ptr = opendir(locationURI.c_str());
	struct dirent *file = NULL;

	if (dir_ptr == NULL)
	{
		setStatusCode(500);
		setState(ERROR);
	}

	autoIndexContent += "<html>";
	autoIndexContent += "<head><title>Index of " + locationURI + "</title></head>";
	autoIndexContent += "<body bgcolor=\"white\">";
	autoIndexContent += "<h1>Index of " + locationURI + "</h1><hr>";
	autoIndexContent += "<pre><a href=\"../\">../</a>";

	while ((file = readdir(dir_ptr)) != NULL)
	{
		fileName = file->d_name;
		autoIndexContent += "<a href=\"" + fileName + "\">" + fileName + "</a>";
		for (size_t idx = 0; idx < 50 - fileName.length(); idx++)
		{
			mBody += " ";
		}
		autoIndexContent += web::getFileTime();
		autoIndexContent += "<br>";
	}
	autoIndexContent += "</pre><hr></body></html>";
	return (autoIndexContent);
}

std::string GETResponse::readIndexPageContent()
{
	size_t nRead;
	int fd = open((getLocationConfig()->getRoot() + mContentLocation).c_str(), O_RDONLY);
	char readBuffer[1025];
	std::string indexPageContent;

	if (fd < 0)
	{
		setStatusCode(404);
		setState(ERROR);
	}
	else
	{
		while (!(nRead = read(fd, readBuffer, 1024)))
		{
			readBuffer[nRead] = '\0';
			indexPageContent = indexPageContent + readBuffer;
		}
		close(fd);
	}
	return (indexPageContent);
}

std::string GETResponse::readTargetContent()
{
	int fd = open(mContentLocation.c_str(), O_RDONLY);
	char readBuffer[1025];
	size_t nRead;
	std::string targetContent = "";

	while (!(nRead = read(fd, readBuffer, 1024)))
	{
		readBuffer[nRead] = '\0';
		targetContent = targetContent + readBuffer;
	}
	close(fd);
	return (targetContent);
}

bool GETResponse::isDirectory(const char *target)
{
	struct stat buf;
	if (stat(target, &buf) < 0)
	{
		setStatusCode(500);
		setState(ERROR);
	}

	if (S_ISDIR(buf.st_mode))
		return (true);
	else
		return (false);
}