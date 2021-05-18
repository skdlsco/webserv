#include "Response.hpp"

std::string const Response::TAG = "Response";

Response::Response(ServerManager &serverManager)
: ServerComponent(serverManager), mServerConfig(NULL), mLocationConfig(NULL), mState(ON_WORKING)
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
		this->mServerConfig = rhs.mServerConfig;
		this->mLocationConfig = rhs.mLocationConfig;
		this->mState = rhs.mState;
	}
	return (*this);
}

void Response::onRepeat()
{

}

std::string *Response::getResponse()
{
	if (mState != DONE)
		return (NULL);
	return (new std::string(createResponseHeader() + createResponseBody()));
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
