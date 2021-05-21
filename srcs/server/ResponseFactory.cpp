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
: mServerManager(serverManager), mRequest(request), mServerConfig(config), mResponseState(METHOD)
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
	checkLocationCGI();
	checkLocationMethodList();
	
	if (mResponseState == ERROR)
		createErrorResponse();
	if (mResponseState == CGI)
		createCGIResponse();
	if (mResponseState == METHOD)
		createMethodResponse();

	setResponseServerConfig(mResponse);
	setResponseLocationConfig(mResponse);
	return (mResponse);
}

void ResponseFactory::checkRequestErrorCode()
{
	if (mRequest.getErrorCode())
		mResponseState = ERROR;
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

	if (currentLocationURI == "")
		mResponseState == ERROR;
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
	bool findMethod = false;
	std::vector<std::string> methodList = mLocationConfig->getAllowMethodList();
	std::string requestMethod = mRequest.getMethod();

	if (mResponseState == ERROR || mResponseState == CGI)
		return ;

	for (std::vector<std::string>::iterator iter = methodList.begin(); iter != methodList.end(); iter++)
	{
		if (requestMethod == *iter)
			findMethod = true;
	}

	if (!findMethod)
		mResponseState = ERROR;
}


void ResponseFactory::createErrorResponse()
{
	if (mResponse)
		delete mResponse;
	// mResponse = new ErrorResponse(mServerManager));
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



void ResponseFactory::setResponseServerConfig(Response *response)
{
	response->setServerConfig(mServerConfig);
}

void ResponseFactory::setResponseLocationConfig(Response *response)
{
	response->setLocationConfig(mLocationConfig);
}
