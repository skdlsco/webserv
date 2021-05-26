#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: Response(serverManager, serverConfig, locationConfig), mContentLocation(""), mBody(""), mState(INDEX_HTML)
{
	if (isDirectory())
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
		int fd = open(getTarget().c_str(), O_RDONLY);

		if (fd < 0)
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
		//TODO
	}
	return (*this);
}

GETResponse::~GETResponse()
{
	
}

std::string GETResponse::createResponseHeader()
{
	std::string responseHeader;

	responseHeader += "Date: " + web::getDate() + "\r\n";
	responseHeader += "Server: webserv (chlee, ina)\r\n";
	responseHeader += "Connection: close\r\n";
	
	responseHeader += "Content-Language: en-US\r\n";
	responseHeader += "Content-Length: " + (mBody.length() + 2);

	if (mState != AUTOINDEX)
		responseHeader += "Content-Location: " + mContentLocation;

	if (mContentLocation.split(~)) //if not found . back -> text/plain
		responseHeader += "Content-Type: text/html\r\n";
	else
		responseHeader += "Content-Type: " + web::mime_type[~] + "\r\n";

	if (mState != AUTOINDEX)
		responseHeader += "Last-Modified: " + get~; // openFile? what func?
}

std::string GETResponse::createResponseBody()
{
	if (mState == INDEX_HTML)
	{
		mContentLocation = getLocationConfig()->getRoot() + getLocationConfig()->getIndexFile();
		mBody = readIndexFile() + "\r\n";
	}
	else if (mState == AUTOINDEX)
	{
		// mContentLocation = //readdir()
		mBody = makeAutoIndexContent() + "\r\n";
	}
	else if (mState == TARGET)
	{
		//target ? root setting?
		mContentLocation = getTarget();
		mBody = readTargetFile() + "\r\n";
	}
}

std::string GETResponse::makeAutoIndexContent()
{
	std::string locationURI = getTarget();
	DIR *dir_ptr = opendir(locationURI.c_str());
	struct dirent *file = NULL;

	if (dir_ptr == NULL)
		// throw()

	mBody += "<html>\r\n";
	mBody += "<head><title>Index of " + locationURI + "</title></head>\r\n";
	mBody += "<body bgcolor=\"white\">\r\n";
	mBody += "<h1>Index of " + locationURI + "</h1><hr>";
	mBody += "<pre><a href=\"../\">../</a>";

	while ((file = readdir(dir_ptr)) != NULL)
	{
		mBody += "<a href=\"" + file->d_name + "\">" + file->d_name + "</a>";
		for (size_t idx = 0; idx < 50 - strlen(file->d_name); idx++)
			mBody += " ";
		mBody += 
	}
	
	mBody += 

}

std::string GETResponse::readIndexFile()
{

}

std::string GETResponse::readTargetFile()
{
	int fd = open(mContentLocation.c_str(), O_RDONLY);




}

bool GETResponse::isDirectory()
{
	struct stat buf;
	if (stat(getTarget().c_str(), &buf) < 0)
		//error

	if (S_ISDIR(buf.st_mode))
		return (true);
	else
		return (false);
}