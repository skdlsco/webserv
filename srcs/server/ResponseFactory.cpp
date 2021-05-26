#include "ResponseFactory.hpp"

std::string const ResponseFactory::TAG = "ResponseFactory";

Response *ResponseFactory::create(ServerManager &serverManager, Request &request, const ServerConfig *config)
{
	try
	{
		ResponseFactory responseFactory = ResponseFactory(serverManager, request, config);

		return (responseFactory.createResponse());
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
	}
	return (NULL);
}

ResponseFactory::ResponseFactory(ServerManager &serverManager, Request &request, const ServerConfig *config)
: mServerManager(serverManager), mResponseState(METHOD), mResponse(NULL),
	mRequest(request), mServerConfig(config), mLocationConfig(NULL), mStatusCode(0)
{
	checkRequestErrorCode();
}


ResponseFactory::~ResponseFactory()
{

}

Response *ResponseFactory::createResponse()
{
	checkRequestErrorCode();
	logger::print(TAG) << mLocationConfig << std::endl;
	checkLocationURI();
	logger::print(TAG) << mLocationConfig << std::endl;
	checkLocationCGI();
	checkLocationMethodList();

	if (mResponseState == ERROR)
		createErrorResponse();
	if (mResponseState == CGI)
		createCGIResponse();
	if (mResponseState == METHOD)
		createMethodResponse();
	if (mResponse)
	{
		mResponse->setTarget(mRequest.getTarget());
		mResponse->setRequestHeader(mRequest.getField());
		mResponse->setRequestBody(mRequest.getBody());
	}
	return (mResponse);
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


void ResponseFactory::createErrorResponse()
{
	if (mResponse)
		delete mResponse;
	mResponse = new ErrorResponse(mServerManager, mServerConfig, mLocationConfig);
	mResponse->setStatusCode(mStatusCode);
}

void ResponseFactory::createCGIResponse()
{
	if (mResponse)
		delete mResponse;
	// mResponse = new CGIResponse(mServerManager));
}

void ResponseFactory::createMethodResponse()
{
	std::string method = mRequest.getMethod();

	/* Meaningless Code */
	if (mResponse)
		delete mResponse;

	/* will changed */
	if (method == web::method[web::GET])
		mResponse = new GETResponse(mServerManager, mServerConfig, mLocationConfig);
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
}
