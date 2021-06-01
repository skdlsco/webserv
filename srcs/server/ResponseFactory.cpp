#include "ResponseFactory.hpp"

std::string const ResponseFactory::TAG = "ResponseFactory";

std::string *ResponseFactory::create(struct sockaddr_in clientAddr,
										Request &request)
{
	std::string *result = NULL;
	Response *response = NULL;
	try
	{
		ResponseFactory responseFactory(clientAddr, request);
		response = responseFactory.createResponse();
		if (response)
			result = response->getResponse();
		if (result == NULL)
		{
			logger::println(TAG, "currentStatusCode: " + web::toString(responseFactory.mStatusCode));
			Response *errorResponse = responseFactory.createErrorResponse();
			if (errorResponse)
			{
				errorResponse->setTarget(request.getTarget());
				errorResponse->setRequestHeader(request.getField());
				errorResponse->setRequestBody(request.getBody());
				if (responseFactory.mResponseState == ERROR)
					errorResponse->setStatusCode(responseFactory.mStatusCode);
				else if (response)
					errorResponse->setStatusCode(response->getStatusCode());
				else
					errorResponse->setStatusCode(500);
			}
			delete response;
			response = errorResponse;
			result = response->getResponse();
		}
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
	}
	logger::print(TAG) << response->getStatusCode() << std::endl;
	delete response;
	return (result);
}

std::string *ResponseFactory::createTimeoutResponse(struct sockaddr_in clientAddr,
														Request &request)
{
	ResponseFactory responseFactory(clientAddr, request);
	Response *errorResponse = responseFactory.createErrorResponse();
	std::string *result = NULL;

	if (errorResponse)
	{
		errorResponse->setTarget(request.getTarget());
		errorResponse->setRequestHeader(request.getField());
		errorResponse->setRequestBody(request.getBody());
		errorResponse->setStatusCode(408);

		result = errorResponse->getResponse();
		delete errorResponse;
	}
	return (result);
}

ResponseFactory::ResponseFactory(struct sockaddr_in clientAddr, Request &request)
: mResponseState(METHOD), mRequest(request), mClientAddr(clientAddr),
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

	logger::print(TAG) << web::toAddr(mClientAddr.sin_addr.s_addr) << " " << mRequest.getMethod() << " " << mRequest.getTarget() << "?" << mRequest.getQuery() << std::endl;
	try
	{
		if (mResponseState == CGI)
		{
			logger::print(TAG) << "is CGI" << std::endl;
			response = createCGIResponse();
		}
		if (mResponseState == METHOD)
		{
			logger::print(TAG) << "is METHOD" << std::endl;
			response = createMethodResponse();
		}
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
	return (new ErrorResponse(mServerConfig, mLocationConfig));
}

Response *ResponseFactory::createCGIResponse()
{
	return (new CGIResponse(mServerConfig, mLocationConfig));
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
