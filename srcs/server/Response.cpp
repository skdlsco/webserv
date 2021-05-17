#include "Response.hpp"

std::string const Response::TAG = "Response";

Response::Response(ServerManager &serverManager)
: ServerComponent(serverManager), mConfig(NULL), mState(ON_WORKING)
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
