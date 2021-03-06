#include "Response.hpp"

std::string const Response::TAG = "Response";

Response::Response(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: mStatusCode(0), mServerConfig(serverConfig), mLocationConfig(locationConfig), mState(START)
{

}

Response::~Response()
{
	mRequestHeader.clear();
}

Response::Response(Response const & copy)
{
	*this = copy;
}

Response &Response::operator=(Response const & rhs)
{
	if (this != &rhs)
	{
		this->mStatusCode = rhs.mStatusCode;;
		this->mTarget = rhs.mTarget;;
		this->mRequestHeader = rhs.mRequestHeader;;
		this->mServerConfig = rhs.mServerConfig;
		this->mLocationConfig = rhs.mLocationConfig;
	}
	return (*this);
}

std::string Response::createResponseLine()
{
	std::string responseline;

	responseline += "HTTP/1.1 ";
	responseline += web::toString(mStatusCode);
	responseline += " ";
	if (mStatusMessage.empty())
		mStatusMessage = web::getStatusMessage(mStatusCode);
	responseline += mStatusMessage;
	responseline += "\r\n";
	return (responseline);
}

std::string Response::createDefaultResponseHeader()
{
	std::string defaultResponseHeader;

	defaultResponseHeader += "Date: " + web::getDate() + "\r\n";
	defaultResponseHeader += "Server: webserv (chlee, ina)\r\n";
	if (isKeepAlive())
		defaultResponseHeader += "Connection: keep-alive\r\n";
	else
		defaultResponseHeader += "Connection: close\r\n";

	return (defaultResponseHeader);
}

int Response::getStatusCode() const
{
	return (mStatusCode);
}

void Response::setStatusCode(int statusCode)
{
	mStatusCode = statusCode;
}

struct sockaddr_in Response::getClientAddr() const
{
	return (mClientAddr);
}

void Response::setClientAddr(struct sockaddr_in clientAddr)
{
	mClientAddr = clientAddr;
}


std::string Response::getTarget() const
{
	return (mTarget);
}

void Response::setTarget(std::string target)
{
	mTarget = target;
}

std::string Response::getTargetContent() const
{
	return (mTargetContent);
}

void Response::setTargetContent(std::string targetContent)
{
	mTargetContent = targetContent;
}

std::string Response::getMethod() const
{
	return (mMethod);
}

void Response::setMethod(std::string method)
{
	mMethod = method;
}

std::string Response::getQuery() const
{
	return (mQuery);
}

void Response::setQuery(std::string query)
{
	mQuery = query;
}

std::map<std::string, std::string> const & Response::getRequestHeader() const
{
	return (mRequestHeader);
}

void Response::setRequestHeader(std::map<std::string, std::string> requestHeader)
{
	mRequestHeader = requestHeader;
}

std::string Response::getRequestBody() const
{
	return (mRequestBody);
}

void Response::setRequestBody(std::string requestBody)
{
	mRequestBody = requestBody;
}

const ServerConfig *Response::getServerConfig() const
{
	return (mServerConfig);
}

void Response::setServerConfig(const ServerConfig *config)
{
	mServerConfig = config;
}

const LocationConfig *Response::getLocationConfig() const
{
	return (mLocationConfig);
}

void Response::setLocationConfig(const LocationConfig *config)
{
	mLocationConfig = config;
}

enum State Response::getState() const
{
	return (mState);
}

void Response::setState(enum State state)
{
	mState = state;
}

bool Response::isKeepAlive() const
{
	return (mIsKeepAlive);
}

void Response::setIsKeepAlive(bool isKeepAlive)
{
	mIsKeepAlive = isKeepAlive;
}

std::string &Response::getResponse()
{
	return (mResponseContent);	
}
