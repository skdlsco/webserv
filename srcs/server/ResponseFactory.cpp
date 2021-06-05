#include "ResponseFactory.hpp"

std::string const ResponseFactory::TAG = "ResponseFactory";

Response *ResponseFactory::create(ServerManager &serverManager, struct sockaddr_in clientAddr,
										Request &request)
{
	Response *response = NULL;
	try
	{
		ResponseFactory responseFactory(serverManager, clientAddr, request);
		response = responseFactory.createResponse();
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
	}
	return (response);
}

Response *ResponseFactory::createCGIResponse()
{
	CGIResponse *response = new CGIResponse(mServerManager, mRequest.getServerConfig(), mRequest.getLocationConfig());
	return (response);
}

ResponseFactory::ResponseFactory(ServerManager &serverManager, struct sockaddr_in clientAddr, Request &request)
: mServerManager(serverManager), mResponseState(METHOD), mRequest(request), mClientAddr(clientAddr),
	mServerConfig(request.getServerConfig()), mLocationConfig(request.getLocationConfig()), mStatusCode(0)
{
	checkResponseType();
}

ResponseFactory::~ResponseFactory()
{

}

Response *ResponseFactory::createResponse()
{
	Response *response = NULL;

	logger::print(TAG) << web::toAddr(mClientAddr.sin_addr.s_addr) << ":" << web::toString(ntohs(mClientAddr.sin_port)) 
						<< " " << mRequest.getMethod() << " " << mRequest.getTarget() << "?" << mRequest.getQuery() << std::endl;
	try
	{
		if (mResponseState == CGI)
			response = createCGIResponse();
		if (mResponseState == ERROR)
			response = createErrorResponse();
		if (mResponseState == METHOD)
			response = createMethodResponse();
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
	}
	if (response)
	{
		response->setTarget(mRequest.getTarget());
		response->setTargetContent(mRequest.getTargetContent());
		response->setRequestHeader(mRequest.getField());
		response->setRequestBody(mRequest.getBody());
		response->setQuery(mRequest.getQuery());
		response->setMethod(mRequest.getMethod());
		response->setClientAddr(mClientAddr);
		response->setIsKeepAlive(mRequest.getErrorCode() == 0);
	}
	return (response);
}

void ResponseFactory::checkResponseType()
{
	if (mRequest.getErrorCode())
	{
		mStatusCode = mRequest.getErrorCode();
		mResponseState = ERROR;
	} else if (mRequest.isCGI())
		mResponseState = CGI;
	else
		mResponseState = METHOD;
}

Response *ResponseFactory::createErrorResponse()
{
	Response *response = new ErrorResponse(mServerConfig, mLocationConfig);
	if (response)
	{
		response->setStatusCode(mStatusCode);
	}
	return (response);
}

Response *ResponseFactory::createMethodResponse()
{
	std::string method = mRequest.getMethod();

	/* will changed */
	if (method == web::method[web::GET])
		return (new GETResponse(mServerConfig, mLocationConfig));
	else if (method == web::method[web::PUT])
		return (new PUTResponse(mServerConfig, mLocationConfig));
	else if (method == web::method[web::HEAD])
		return (new HEADResponse(mServerConfig, mLocationConfig));
	else if (method == web::method[web::POST])
		return (new POSTResponse(mServerConfig, mLocationConfig));
	else if (method == web::method[web::OPTIONS])
		return (new OPTIONSResponse(mServerConfig, mLocationConfig));
	else if (method == web::method[web::DELETE])
		return (new DELETEResponse(mServerConfig, mLocationConfig));
	return (NULL);
}
