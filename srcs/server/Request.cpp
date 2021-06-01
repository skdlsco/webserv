#include "Request.hpp"

std::string const Request::TAG = "Request";
std::string const Request::HTTP_VERSION = "HTTP/1.1";

Request::Request(std::vector<ServerConfig *> const & configVec)
: mConfigVec(configVec), mServerConfig(web::getDefaultServerConfig(configVec)), mLocationConfig(NULL),
	mAnalyzeLevel(REQUEST_LINE), mHasBody(false), mIsChunked(false), mIsReadData(false),
	mContentLength(0), mErrorCode(0)
{

}

Request::~Request()
{

}

Request::Request(Request const & copy) : mConfigVec(copy.mConfigVec)
{
	*this = copy;
}

Request &Request::operator=(Request const & rhs)
{
	if (this != &rhs)
	{
		mServerConfig = rhs.mServerConfig;
		mLocationConfig = rhs.mLocationConfig;
		mAnalyzeLevel = rhs.mAnalyzeLevel;
		mBuffer = rhs.mBuffer;
		mBody = rhs.mBody;
		mMethod = rhs.mMethod;
		mTarget = rhs.mTarget;
		mField = rhs.mField;
		mErrorCode = rhs.mErrorCode;
	}
	return (*this);
}

void Request::badRequest(int errorCode)
{
	mErrorCode = errorCode;
	mAnalyzeLevel = DONE;
}

void Request::appendChunkedBody()
{
	while (mAnalyzeLevel == BODY && !mBuffer.empty())
	{
		if (mIsReadData)
		{
			if (mBuffer.size() < static_cast<unsigned long>(mContentLength))
				break ;
			mBody.append(mBuffer.substr(0, mContentLength));
			mBuffer.erase(0, mContentLength + 2);
			if (mBody.length() > mLocationConfig->getClientMaxBodySize())
				badRequest(413);
			if (mContentLength == 0)
				mAnalyzeLevel = DONE;
			mIsReadData = false;
		} else
		{
			std::string line;
			size_t lineIndex = mBuffer.find("\r\n");
			if (lineIndex != std::string::npos)
			{
				line = mBuffer.substr(0, lineIndex);
				logger::println(TAG, "initial Buffer: |" + mBuffer + "|");
				mBuffer.erase(0, lineIndex + 2);
				logger::println(TAG, "Erase After Buffer: |" + mBuffer + "|");
				if (line.empty())
					badRequest(400);
				mContentLength = web::axtoi(line.c_str());
				if (mContentLength < 0)
					badRequest(400);
				mIsReadData = true;
			}
			else
				break ;
		}
	}
}

void Request::appendContentBody()
{
	if (mBody.size() + mBuffer.size() > static_cast<unsigned long>(mContentLength))
		mBody.append(mBuffer.substr(0, mContentLength - mBody.size()));
	else
		mBody.append(mBuffer);
	if (mBody.size() >= static_cast<unsigned long>(mContentLength))
		mAnalyzeLevel = DONE;
	mBuffer.clear();
}

void Request::analyzeBody()
{
	if (mErrorCode || !mHasBody)
	{
		mAnalyzeLevel = DONE;
		mBuffer.clear();
		return ;
	}
	if (mIsChunked)
		appendChunkedBody();
	else
		appendContentBody();
}

void Request::checkHost()
{
	FieldIter iter = mField.find("HOST");

	if (iter != mField.end())
		mServerConfig = web::getConfigMatchedWithHost(iter->second, mConfigVec);
	else
		badRequest(400);
}

void Request::checkContentLength()
{
	FieldIter iter = mField.find("CONTENT-LENGTH");

	if (iter != mField.end())
	{
		mContentLength = web::atoi(iter->second.c_str());
		if (mContentLength < 0)
			badRequest(400);
		else if (mLocationConfig->getClientMaxBodySize() < static_cast<unsigned long>(mContentLength))
			badRequest(413);
		mHasBody = true;
	};
}

void Request::checkTransferEncoding()
{
	FieldIter iter = mField.find("TRANSFER-ENCODING");

	if (iter != mField.end())
	{

		if (iter->second == "chunked")
			mIsChunked = true;
		else
			badRequest(400);
		mContentLength = 0;
		mHasBody = true;
	}
}

void Request::checkLocationURI()
{
	std::map<std::string, LocationConfig *> locationConfig = mServerConfig->getLocationList();
	std::vector<std::string> locationURIList;
	std::string currentLocationURI = "";
	std::string requestTarget = web::removeConsecutiveDuplicate(mTarget + '/', '/');

	if (mErrorCode)
		return ;

	for (std::map<std::string, LocationConfig *>::iterator iter = locationConfig.begin(); iter != locationConfig.end(); iter++)
	{
		locationURIList.push_back((*iter).first);
	}
	for (std::vector<std::string>::iterator iter = locationURIList.begin(); iter != locationURIList.end(); iter++)
	{
		if ((requestTarget.find(*iter) != std::string::npos) && ((*iter).length() > currentLocationURI.length()))
			currentLocationURI = *iter;
	}

	/* 404 not found */
	if (currentLocationURI == "")
		badRequest(404);
	else
	{
		mLocationConfig = locationConfig[currentLocationURI];
		if (mTarget.length() > currentLocationURI.length())
			mTargetContent = mTarget.substr(currentLocationURI.length() - 1);
		else
			mTargetContent = "";
		logger::print(TAG) << "currentLocationURI: " << currentLocationURI << std::endl;
		logger::print(TAG) << "mTargetContent: " << mTargetContent << std::endl;
	}
}


void Request::checkLocationCGI()
{
	if (mErrorCode)
		return ;

	mIsCGI = false;
	if (mLocationConfig->getCGIPath() != "")
	{
		std::vector<std::string> CGIExtensionList = mLocationConfig->getCGIExtensionList();
		/* substr error */
		if (mLocationConfig->getURI().length() > mTarget.length())
			return ;
		std::string targetFileExtension = mTarget.substr(mLocationConfig->getURI().length());
		size_t dotIdx = targetFileExtension.find('.');
		if (dotIdx == std::string::npos)
			return ;
		targetFileExtension.erase(0, dotIdx);
		size_t slashIdx = targetFileExtension.find("/");
		if (slashIdx != std::string::npos)
			targetFileExtension.erase(slashIdx);
		for (std::vector<std::string>::iterator iter = CGIExtensionList.begin(); iter != CGIExtensionList.end(); iter++)
		{
			if (targetFileExtension == *iter)
			{
				mIsCGI = true;
				return ;
			}
		}
	}
}

void Request::checkLocationMethodList()
{
	if (mErrorCode)
		return ;

	bool isFindMethod = false;
	std::string requestMethod = getMethod();
	std::vector<std::string> methodList = mLocationConfig->getAllowMethodList();;

	if (mIsCGI)
		methodList = mLocationConfig->getCGIMethodList();

	for (std::vector<std::string>::iterator iter = methodList.begin(); iter != methodList.end(); iter++)
	{
		if (requestMethod == *iter)
			isFindMethod = true;
	}

	/* to check CGI */
	if (!isFindMethod && mIsCGI)
	{
		mIsCGI = false;
		checkLocationMethodList();
		return ;
	}

	/* 405 method not allowed */
	if (!isFindMethod)
		badRequest(405);
}

void Request::checkHeader()
{
	checkHost();
	checkLocationURI();
	checkLocationCGI();
	checkLocationMethodList();
	checkContentLength();
	checkTransferEncoding();
	// std::map<std::string, std::string>::iterator iter = mField.begin();
	// for (; iter != mField.end(); iter++)
	// {
	// 	logger::print(TAG) << iter->first << ": " << iter->second << std::endl;
	// }
}

bool Request::isValidMethod(std::string method)
{
	for (size_t i = 0; i < NUM_METHOD_LIST; i++)
	{
		if (method == web::method[i])
			return (true);
	}
	return (false);
}

bool Request::isValidTarget(std::string target)
{
	return (target[0] == '/');
}

void Request::analyzeRequestLine(std::string line)
{
	std::vector<std::string> lineElements = web::split(line, std::string(" \t"));

	if (lineElements.size() != 3 || !isValidMethod(lineElements[0]) ||
		!isValidTarget(lineElements[1])	|| lineElements[2] != HTTP_VERSION)
	{
		badRequest(400);
		return ;
	}
	mMethod = lineElements[0];
	std::string url = web::removeConsecutiveDuplicate(lineElements[1], '/');
	size_t queryIdx = url.find("?");
	mTarget = url;
	if (queryIdx != url.npos)
	{
		mTarget = url.substr(0, queryIdx);
		mQuery = url.substr(queryIdx + 1);
	}
	mAnalyzeLevel = HEADER;
}

void Request::analyzeHeaderField(std::string line)
{
	size_t colonIndex = line.find_first_of(":");

	if (colonIndex == std::string::npos)
	{
		badRequest(400);
		return ;
	}
	std::string key = line.substr(0, colonIndex);
	std::string value = line.substr(colonIndex + 1);

	web::trim(value);
	web::toUpper(key);
	if (mField.find(key) != mField.end())
	{
		badRequest(400);
		return ;
	}
	mField.insert(std::pair<std::string, std::string>(key, value));
}

void Request::analyzeHeader()
{
	std::string line;
	std::size_t lineIndex = mBuffer.find("\r\n");

	while (lineIndex != std::string::npos)
	{
		line = mBuffer.substr(0, lineIndex);
		mBuffer.erase(0, lineIndex + 2);
		if (line.empty())
		{
			if (mAnalyzeLevel == HEADER && !mErrorCode)
			{
				checkHeader();
				mAnalyzeLevel = BODY;
				return ;
			}
		}
		else if (mAnalyzeLevel == REQUEST_LINE)
			analyzeRequestLine(line);
		else if (mAnalyzeLevel == HEADER)
			analyzeHeaderField(line);
		lineIndex = mBuffer.find("\r\n");
	}
}

void Request::analyzeBuffer(char *buffer)
{
	mBuffer.append(buffer);
	if (mAnalyzeLevel == REQUEST_LINE || mAnalyzeLevel == HEADER)
		analyzeHeader();
	if (mAnalyzeLevel == BODY)
		analyzeBody();
	if (buffer[0] == '\0')
		mAnalyzeLevel = DONE;
}

std::vector<ServerConfig *> const &Request::getConfigVec() const
{
	return (mConfigVec);
}

ServerConfig *Request::getServerConfig() const
{
	return (mServerConfig);
}

LocationConfig *Request::getLocationConfig() const
{
	return (mLocationConfig);
}

bool Request::isCGI() const
{
	return (mIsCGI);
}

enum Request::AnalyzeLevel Request::getAnalyzeLevel() const
{
	return (mAnalyzeLevel);
}

std::string Request::getBuffer() const
{
	return (mBuffer);
}

std::string Request::getBody() const
{
	return (mBody);
}

std::string Request::getMethod() const
{
	return (mMethod);
}

std::string Request::getTarget() const
{
	return (mTarget);
}

std::string Request::getTargetContent() const
{
	return (mTargetContent);
}

std::string Request::getQuery() const
{
	return (mQuery);
}

std::map<std::string, std::string> Request::getField() const
{
	return (mField);
}

int Request::getErrorCode() const
{
	return (mErrorCode);
}
