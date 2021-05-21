#include "ResponseFactory.hpp"

std::string const ResponseFactory::TAG = "ResponseFactory";

Response *ResponseFactory::Create(ServerManager &serverManager, Request &request, const ServerConfig *config)
{
	try
	{
		ResponseFactory responseFactory = ResponseFactory(serverManager, request, config);
		Response *response = responseFactory.createResponse();

		return (response);
		
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
	}
	return (NULL);
}

ResponseFactory::ResponseFactory(ServerManager &serverManager, Request &request, const ServerConfig *config)
: mServerManager(serverManager), mRequest(request), mServerConfig(config), mState(WORKING), mCGI(false)
{
	checkRequestErrorCode();
}


ResponseFactory::~ResponseFactory()
{

}

Response *ResponseFactory::createResponse()
{
	checkRequestErrorCode();
	checkLocationURI();
	checkLocationMethodList();
	if (mState != DONE)
	{
		createDetailResponse();
		checkTargetCGI();
		setResponseServerConfig(mResponse);
		setResponseLocationConfig(mResponse);
		setResponseCGI(mResponse);
	}
	return (mResponse);
}

void ResponseFactory::checkRequestErrorCode()
{
	if (mRequest.getErrorCode())
	{
		createErrorResponse();
		mState = DONE;
	}
}

void ResponseFactory::checkLocationURI()
{
	std::map<std::string, LocationConfig *> locationConfig = mServerConfig->getLocationList();
	std::vector<std::string> locationURIList;
	std::string currentLocationURI = "";
	std::string requestTarget = mRequest.getTarget();

	if (mState == DONE)
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

	if (currentLocationURI == "")
		createErrorResponse();
	else
		mLocationConfig = locationConfig[currentLocationURI];
}

void ResponseFactory::checkLocationMethodList()
{
	bool findMethod = false;
	std::vector<std::string> methodList = mLocationConfig->getAllowMethodList();
	std::string requestMethod = mRequest.getMethod();

	for (std::vector<std::string>::iterator iter = methodList.begin(); iter != methodList.end(); iter++)
	{
		if (requestMethod == *iter)
			findMethod = true;
	}

	if (!findMethod)
	{
		createErrorResponse();
		mState = DONE;
	}
}

void ResponseFactory::checkTargetCGI()
{
	std::vector<std::string> CGIExtensionList;
	std::string targetBackElement;
	std::string targetFileExtension;
	size_t dotIndex;

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
				mCGI = true;
				return ;
			}
		}
	}
}

void ResponseFactory::createDetailResponse()
{
	std::string method = mRequest.getMethod();

	/* Meaningless Code */
	if (mResponse)
		delete mResponse;

	/* will changed */
	// if (method == web::method[web::Method::GET])
	// 	// mResponse = new GETResponse(mServerManager));
	// else if (method == web::method[web::Method::HEAD])
	// 	// mResponse = new HEADResponse(mServerManager));
	// else if (method == web::method[web::Method::PUT])
	// 	// mResponse = new PUTResponse(mServerManager));
	// else if (method == web::method[web::Method::POST])
	// 	// mResponse = new POSTResponse(mServerManager));
	// else if (method == web::method[web::Method::OPTIONS])
	// 	// mResponse = new OPTIONSResponse(mServerManager));
	// else if (method == web::method[web::Method::DELETE])
	// 	// mResponse = new DELETEResponse(mServerManager));
}

void ResponseFactory::createErrorResponse()
{
	if (mResponse)
		delete mResponse;
	// mResponse = new ErrorResponse(mServerManager));
}

void ResponseFactory::setResponseServerConfig(Response *response)
{
	response->setServerConfig(mServerConfig);
}

void ResponseFactory::setResponseLocationConfig(Response *response)
{
	response->setLocationConfig(mLocationConfig);
}

void ResponseFactory::setResponseCGI(Response *response)
{
	response->setCGIResponse(mCGI);
}