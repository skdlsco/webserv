#include "Request.hpp"

std::string const Request::TAG = "Request";
std::string const Request::HTTP_VERSION = "HTTP/1.1";

Request::Request() : mAnalyzeLevel(REQUEST_LINE), mErrorCode(0)
{

}

Request::~Request()
{

}

Request::Request(Request const & copy)
{
	*this = copy;
}

Request &Request::operator=(Request const & rhs)
{
	if (this != &rhs)
	{
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

void Request::analyzeBody()
{
	if (mErrorCode)
	{
		mAnalyzeLevel = DONE;
		return ;
	}
	// check content length > body + buffer
	mBody.append(mBuffer);
	mBuffer.clear();
}

static bool isInvalidMethod(std::string method)
{
	for (size_t i = 0; i < NUM_METHOD_LIST; i++)
	{
		if (method == web::method[i])
			return (true);
	}
	return (false);
}

void Request::analyzeRequestLine(std::string line)
{
	// origin-form 이외 form은 ...
	std::vector<std::string> lineElements = web::split(line, std::string(" "));

	if (lineElements.size() != 3 || isInvalidMethod(lineElements[0]) ||
		lineElements[2] != HTTP_VERSION)
	{
		mErrorCode = 400;
		mAnalyzeLevel = DONE;
		return ;
	}
	mMethod = lineElements[0];
	mTarget = lineElements[1];
	mAnalyzeLevel = HEADER;
}

void Request::analyzeHeaderField(std::string line)
{
	size_t colonIndex = line.find_first_of(":");

	if (colonIndex == std::string::npos)
	{
		mAnalyzeLevel = DONE;
		mErrorCode = 400;
		return ;
	}
	std::string key = line.substr(0, colonIndex);
	std::string value = line.substr(colonIndex + 1);

	web::trim(value);
	if (mField.find(key) != mField.end())
	{
		mAnalyzeLevel = DONE;
		mErrorCode = 400;
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
			if (mAnalyzeLevel == HEADER)
			{
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
	if (!buffer)
	{
		mAnalyzeLevel = DONE;
		mErrorCode = 400;
		return ;
	}
	mBuffer.append(buffer);
	if (mAnalyzeLevel == REQUEST_LINE || mAnalyzeLevel == HEADER)
		analyzeHeader();
	if (mAnalyzeLevel == BODY)
		analyzeBody();
	if (buffer[0] == '\0')
		mAnalyzeLevel = DONE;
}

enum AnalyzeLevel Request::getAnalyzeLevel() const
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
