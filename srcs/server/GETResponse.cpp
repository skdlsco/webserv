#include "GETResponse.hpp"

std::string const GETResponse::TAG = "GETResponse";

GETResponse::GETResponse(ServerManager &serverManager, const ServerConfig * serverConfig, const LocationConfig * locationConfig, bool autoIndex)
: Response(serverManager, serverConfig, locationConfig), mIsAutoIndex(autoIndex)
{

}

GETResponse::GETResponse(GETResponse const & copy)
: Response(copy)
{
	
}

GETResponse &GETResponse::operator=(GETResponse const & rhs)
{
	if (this != &rhs)
	{
		//TODO
	}
	return (*this);
}

std::string GETResponse::createResponseHeader()
{
	std::string responseHeader;
	//put default header
	responseHeader += "Date: " + web::getDate() + "\r\n";
	responseHeader += "Server: webserv (chlee, ina)\r\n";
	responseHeader += "Connection: close\r\n";
	responseHeader += "Content-Language: en-US\r\n";
	// responseHeader += "Content-Length: " + web::toString(mFile.getBuffer().size() + 2)


	//put get header
	return (responseHeader);
}

std::string GETResponse::createResponseBody()
{
	std::string responseBody;

	if (mIsAutoIndex)
	{

	}
	else
	{

	}

	return (responseBody);
}