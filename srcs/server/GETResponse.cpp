#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mPageState(INDEX_HTML), mContentLocation("")
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
		mPageState = rhs.mPageState;
	}
	return (*this);
}

GETResponse::~GETResponse()
{

}

void GETResponse::errorExcept()
{
	mResponseContent = ErrorResponse::getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}

void GETResponse::run()
{
	initState();
	if (getStatusCode() != 0)
	{
		errorExcept();
		return ;
	}
	try
	{
		initContentLocation();
		std::string responseBody = appendResponseBody();
		if (getStatusCode() == 0)
			setStatusCode(200);
		appendResponseHeader(responseBody);
		mResponseContent += responseBody;
		if (getStatusCode() != 200)
		{
			errorExcept();
			return ;
		}
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		errorExcept();
	}
	setState(DONE);
}

void GETResponse::appendResponseHeader(std::string const & responseBody)
{
	/* default header */
	mResponseContent += createResponseLine();
	mResponseContent += createDefaultResponseHeader();

	/* content part */
	mResponseContent += "Content-Language: en-US\r\n";
	mResponseContent += "Content-Length: " + web::toString(responseBody.length()) + "\r\n";

	if (mPageState != AUTOINDEX)
		mResponseContent += "Content-Location: " + mContentLocation + "\r\n";

	if (mContentLocation.find_last_of('.') != std::string::npos)
		mResponseContent += "Content-Type: " + web::getMIMEType(mContentLocation.substr(mContentLocation.find_last_of('.'))) + "\r\n";
	else if (mPageState == AUTOINDEX)
		mResponseContent += "Content-Type: text/html\r\n";
	else
		mResponseContent += "Content-Type: text/plain\r\n";

	if (mPageState != AUTOINDEX)
		mResponseContent += "Last-Modified: " + web::getLastModifiedTime(mContentLocation) + "\r\n";

	mResponseContent += "\r\n";
}

void GETResponse::initState()
{
	std::string contentLocation = getLocationConfig()->getRoot() + getTargetContent();

	contentLocation = web::removeConsecutiveDuplicate(contentLocation, '/');
	if (isDirectory(contentLocation.c_str()))
	{
		if (contentLocation[contentLocation.length() - 1] != '/')
			contentLocation += "/";
		std::string indexPath = contentLocation + getLocationConfig()->getIndexFile();

		/* 한번 더 폴더일 가능성이 있습니다. */
		if (web::isPathExist(indexPath.c_str()) && !isDirectory(indexPath.c_str())) 
			mPageState = INDEX_HTML;
		else if (getLocationConfig()->isAutoIndex())
			mPageState = AUTOINDEX;
		else
			setStatusCode(404);
	}
	else if (web::isPathExist(contentLocation.c_str()))
			mPageState = TARGET;
	else
		setStatusCode(404);

}

void GETResponse::initContentLocation()
{
	if (mPageState == INDEX_HTML)
		mContentLocation = getLocationConfig()->getRoot() + getTargetContent() + "/" + getLocationConfig()->getIndexFile();
	else if (mPageState == TARGET)
		mContentLocation = getLocationConfig()->getRoot() + getTargetContent();
	else if (mPageState == AUTOINDEX)
		mContentLocation = getLocationConfig()->getRoot() + getTargetContent() + "/";
	mContentLocation = web::removeConsecutiveDuplicate(mContentLocation, '/');
}

std::string GETResponse::appendResponseBody()
{
	if (mPageState == AUTOINDEX)
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
