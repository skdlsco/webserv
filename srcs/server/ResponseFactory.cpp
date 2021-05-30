#include "ResponseFactory.hpp"

std::string const ResponseFactory::TAG = "ResponseFactory";

std::string *ResponseFactory::create(struct sockaddr_in clientAddr,
										Request &request,
										const ServerConfig *config)
{
	std::string *result = NULL;
	Response *response = NULL;
	try
	{
		ResponseFactory responseFactory(clientAddr, request, config);
		response = responseFactory.createResponse();
		if (response)
			result = response->getResponse();
		if (result == NULL)
		{
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
	delete response;
	return (result);
}

std::string *ResponseFactory::createTimeoutResponse(struct sockaddr_in clientAddr,
														Request &request,
														const ServerConfig *config)
{
	ResponseFactory responseFactory(clientAddr, request, config);
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

ResponseFactory::ResponseFactory(struct sockaddr_in clientAddr, Request &request, const ServerConfig *config)
: mResponseState(METHOD), mRequest(request), mClientAddr(clientAddr),
	mServerConfig(config), mLocationConfig(NULL), mStatusCode(0)
{
	checkRequestErrorCode();
}


ResponseFactory::~ResponseFactory()
{

}

Response *ResponseFactory::createResponse()
{
	Response *response = NULL;

	checkRequestErrorCode();
	checkLocationURI();
	checkLocationCGI();
	checkLocationMethodList();
	logger::print(TAG) << web::toAddr(mClientAddr.sin_addr.s_addr) << " " << mRequest.getMethod() << " " << mRequest.getTarget() << mRequest.getQuery() << std::endl;
	try
	{
		if (mResponseState == CGI)
			response = createCGIResponse();
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
		response->setRequestHeader(mRequest.getField());
		response->setRequestBody(mRequest.getBody());
		response->setQuery(mRequest.getQuery());
		response->setMethod(mRequest.getMethod());
		response->setClientAddr(mClientAddr);
	}
	return (response);
}

void ResponseFactory::checkRequestErrorCode()
{
	/* 400 bad request */
	if (mRequest.getErrorCode())
	{
		mResponseState = ERROR;
		mStatusCode = 400;
	}
}

void ResponseFactory::checkLocationURI()
{
	std::map<std::string, LocationConfig *> locationConfig = mServerConfig->getLocationList();
	std::vector<std::string> locationURIList;
	std::string currentLocationURI = "";
	std::string requestTarget = mRequest.getTarget();

	if (mResponseState == ERROR)
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
	{
		mResponseState = ERROR;
		mStatusCode = 404;
	}
	else
		mLocationConfig = locationConfig[currentLocationURI];
}

void ResponseFactory::checkLocationCGI()
{
	std::vector<std::string> CGIExtensionList;
	std::string targetBackElement;
	std::string targetFileExtension;
	size_t dotIndex;

	if (mResponseState == ERROR)
		return ;

	if (mLocationConfig->getCGIPath() != "")
	{
		CGIExtensionList = mLocationConfig->getCGIExtensionList();
		for (std::vector<std::string>::iterator iter = CGIExtensionList.begin(); iter != CGIExtensionList.end(); iter++)
		{
			targetBackElement = web::split(mRequest.getTarget(), "/").back();
			dotIndex = targetBackElement.find('.');
			if (dotIndex != std::string::npos)
				targetFileExtension = targetBackElement.substr(dotIndex);
			if (targetFileExtension == *iter)
			{
				mResponseState = CGI;
				return ;
			}
		}
	}
}

void ResponseFactory::checkLocationMethodList()
{
	if (mResponseState == ERROR || mResponseState == CGI)
		return ;

	bool findMethod = false;
	std::vector<std::string> methodList = mLocationConfig->getAllowMethodList();
	std::string requestMethod = mRequest.getMethod();

	for (std::vector<std::string>::iterator iter = methodList.begin(); iter != methodList.end(); iter++)
	{
		if (requestMethod == *iter)
			findMethod = true;
	}

	/* 405 method not allowed */
	if (!findMethod)
	{
		mResponseState = ERROR;
		mStatusCode = 405;
	}
}

Response *ResponseFactory::createErrorResponse()
{
	return (new ErrorResponse(mServerConfig, mLocationConfig));
}

Response *ResponseFactory::createCGIResponse()
{
	if (mResponseState != CGI)
		return (NULL);
	return (new CGIResponse(mServerConfig, mLocationConfig));
	// return (new CGIResponse(mServerManager)));
	return (NULL);
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
