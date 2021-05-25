#include "Response.hpp"

std::string const Response::TAG = "Response";

Response::Response(ServerManager &serverManager, const ServerConfig * serverConfig,
					const LocationConfig * locationConfig)
: ServerComponent(serverManager), mStatusCode(0), mServerConfig(serverConfig),
	mLocationConfig(locationConfig), mState(ON_WORKING)
{

}

Response::~Response()
{

}

Response::Response(Response const & copy) : ServerComponent(copy.getServerManager())
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
		this->mState = rhs.mState;
	}
	return (*this);
}

void Response::onRepeat()
{

}

std::string Response::createResponseLine()
{
	std::string responseline;

	responseline += "HTTP/1.1 ";
	responseline += web::toString(mStatusCode);
	if (mStatusMessage.empty())
		mStatusMessage = web::getStatusMessage(mStatusCode);
	responseline += mStatusMessage;
	responseline += "\r\n";
	return (responseline);
}

std::string *Response::getResponse()
{
	if (mState != DONE)
		return (NULL);
	return (new std::string(createResponseLine() + createResponseHeader() + createResponseBody()));
}

int Response::getStatusCode() const
{
	return (mStatusCode);
}

void Response::setStatusCode(int statusCode)
{
	mStatusCode = statusCode;
}

std::string Response::getTarget() const
{
	return (mTarget);
}

void Response::setTarget(std::string target)
{
	mTarget = target;
}

std::map<std::string, std::string> Response::getRequestHeader() const
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

Response::ResponseState Response::getState() const
{
	return (mState);
}

void Response::setState(Response::ResponseState state)
{
	mState = state;
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
