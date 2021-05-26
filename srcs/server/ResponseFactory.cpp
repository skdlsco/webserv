#include "ResponseFactory.hpp"

std::string const ResponseFactory::TAG = "ResponseFactory";

std::string *ResponseFactory::create(Request &request, const ServerConfig *config)
{
	std::string *result = NULL;
	Response *response = NULL;
	try
	{
		ResponseFactory responseFactory(request, config);
		response = responseFactory.createResponse();
		if (response)
		{
			result = response->getResponse();
		}
		if (result == NULL)
		{
			Response *errorResponse = responseFactory.createErrorResponse();
			if (errorResponse)
			{
				errorResponse->setTarget(request.getTarget());
				errorResponse->setRequestHeader(request.getField());
				errorResponse->setRequestBody(request.getBody());
			}
			if (responseFactory.mResponseState == ERROR)
				errorResponse->setStatusCode(responseFactory.mStatusCode);
			else if (response)
				errorResponse->setStatusCode(response->getStatusCode());
			else
				errorResponse->setStatusCode(500);
			delete response;
			response = errorResponse;
			result = response->getResponse();
		}
		return (result);
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
	}
	delete response;
	return (result);
}

ResponseFactory::ResponseFactory(Request &request, const ServerConfig *config)
: mResponseState(METHOD), mResponse(NULL),
	mRequest(request), mServerConfig(config), mLocationConfig(NULL), mStatusCode(0)
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
	logger::print(TAG) << mLocationConfig->getCGIPath() << std::endl;
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
	mResponse = new ErrorResponse(mServerConfig, mLocationConfig);
	return (mResponse);
}

Response *ResponseFactory::createCGIResponse()
{
	if (mResponseState != CGI)
		return (NULL);
	// return (new CGIResponse(mServerManager)));
	return (NULL);
}

Response *ResponseFactory::createMethodResponse()
{
	std::string method = mRequest.getMethod();

	/* Meaningless Code */
	if (mResponseState != METHOD)
		return (NULL);

	/* will changed */
	// if (method == web::method[web::Method::GET])
	// 	// mResponse = new GETResponse(mServerManager, mServerConfig, mLocationConfig));
	// else if (method == web::method[web::Method::HEAD])
	// 	// mResponse = new HEADResponse(mServerManager, mServerConfig, mLocationConfig));
	// else if (method == web::method[web::Method::PUT])
	// 	// mResponse = new PUTResponse(mServerManager, mServerConfig, mLocationConfig));
	// else if (method == web::method[web::Method::POST])
	// 	// mResponse = new POSTResponse(mServerManager, mServerConfig, mLocationConfig));
	// else if (method == web::method[web::Method::OPTIONS])
	// 	// mResponse = new OPTIONSResponse(mServerManager, mServerConfig, mLocationConfig));
	// else if (method == web::method[web::Method::DELETE])
	// 	// mResponse = new DELETEResponse(mServerManager, mServerConfig, mLocationConfig));
	return (NULL);
}
