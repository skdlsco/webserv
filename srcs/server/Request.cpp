#include "Request.hpp"

std::string const Request::TAG = "Request";
std::string const Request::HTTP_VERSION = "HTTP/1.1";

Request::Request(std::vector<ServerConfig *> const & configVec)
: mConfigVec(configVec), mConfig(web::getDefaultServerConfig(configVec)), mAnalyzeLevel(REQUEST_LINE),
	mHasBody(false), mIsChunked(false), mIsReadData(false),
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
		mConfig = rhs.mConfig;
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

void Request::badRequest()
{
	mErrorCode = 400;
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
			mBuffer.erase(0, mContentLength);
			if (mContentLength == 0)
				mAnalyzeLevel = DONE;
			mIsReadData = false;
		} else
		{
			std::string line;
			std::size_t lineIndex = mBuffer.find("\r\n");
			if (lineIndex != std::string::npos)
			{
				line = mBuffer.substr(0, lineIndex);
				if (line.empty())
					badRequest();
				mContentLength = web::axtoi(line.c_str());
				if (mContentLength < 0)
					badRequest();
			} else
				badRequest();
			mIsReadData = true;
		}
	}
}

void Request::appendContentBody()
{
	if (mBody.size() + mBuffer.size() > static_cast<unsigned long>(mContentLength))
		mBody.append(mBuffer.substr(0, mContentLength - mBody.size()));
	else
		mBody.append(mBuffer);
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
	{
		mConfig = web::getConfigMatchedWithHost(iter->second, mConfigVec);
	} else
		badRequest();
}

void Request::checkContentLength()
{
	FieldIter iter = mField.find("CONTENT-LENGTH");

	if (iter != mField.end())
	{
		mContentLength = web::atoi(iter->second.c_str());
		if (mContentLength < 0 || mConfig->getClientMaxBodySize() < static_cast<unsigned long>(mContentLength))
			badRequest();
		mHasBody = true;
	};
}

void Request::checkTransferEncoding()
{
	FieldIter iter = mField.find("TRANSFER-ENCODING");

	if (iter != mField.end())
	{
		if (iter->second == "Chunked")
			mIsChunked = true;
		else
			badRequest();
		mContentLength = 0;
		mHasBody = true;
	}
}

void Request::checkHeaderForBody()
{
	checkHost();
	checkContentLength();
	checkTransferEncoding();
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

std::string Request::createTarget(std::string input)
{
	std::string result;
	char prev = ' ';

	for (std::string::iterator iter = input.begin(); iter < input.end(); iter++)
	{
		if (!(prev == '/' && *iter == '/'))
			result += *iter;
		prev = *iter;
	}
	return (result);
}

void Request::analyzeRequestLine(std::string line)
{
	std::vector<std::string> lineElements = web::split(line, std::string(" \t"));

	if (lineElements.size() != 3 || !isValidMethod(lineElements[0]) ||
		!isValidTarget(lineElements[1])	|| lineElements[2] != HTTP_VERSION)
	{
		badRequest();
		return ;
	}
	mMethod = lineElements[0];
	mTarget = createTarget(lineElements[1]);
	mAnalyzeLevel = HEADER;
}

void Request::analyzeHeaderField(std::string line)
{
	size_t colonIndex = line.find_first_of(":");

	if (colonIndex == std::string::npos)
	{
		badRequest();
		return ;
	}
	std::string key = line.substr(0, colonIndex);
	std::string value = line.substr(colonIndex + 1);

	web::trim(value);
	if (mField.find(key) != mField.end())
	{
		badRequest();
		return ;
	}
	web::toUpper(key);
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
				checkHeaderForBody();
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

ServerConfig *Request::getConfig() const
{
	return (mConfig);
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

std::map<std::string, std::string> Request::getField() const
{
	return (mField);
}

int Request::getErrorCode() const
{
	return (mErrorCode);
}
