#include "OPTIONSResponse.hpp"

std::string const OPTIONSResponse::TAG = "OPTIONSResponse";

OPTIONSResponse::OPTIONSResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig)
{

}

OPTIONSResponse::OPTIONSResponse(OPTIONSResponse const & copy)
: Response(copy)
{
	*this = copy;
}

OPTIONSResponse &OPTIONSResponse::operator=(OPTIONSResponse const & rhs)
{
	Response::operator=(rhs);
	return (*this);
}

OPTIONSResponse::~OPTIONSResponse()
{

}

std::string *OPTIONSResponse::getResponse()
{
	std::string *responseContent;

	try
	{
		responseContent = new std::string();
		if (responseContent)
		{
			*responseContent += createResponseLine();
			appendResponseHeader(*responseContent);
			appendResponseBody(*responseContent);
		}
		setStatusCode(200);
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		delete responseContent;
		responseContent = NULL;
	}
	return (responseContent);
}

void OPTIONSResponse::appendResponseHeader(std::string & responseContent)
{
	/* default header */
	responseContent += "Date: " + web::getDate() + "\r\n";
	responseContent += "Server: webserv (chlee, ina)\r\n";
	responseContent += "Connection: close\r\n";

	/* content part */
	responseContent += "Content-Length: 0\r\n";
	responseContent += "Content-Language: en-US\r\n";

	responseContent += "Allow: ";
	for (size_t idx = 0; idx < getLocationConfig()->getAllowMethodList().size(); idx++)
	{
		responseContent += getLocationConfig()->getAllowMethodList()[idx];
		if (idx < getLocationConfig()->getAllowMethodList().size() - 1)
			responseContent += " ";
	}
	responseContent += "\r\n";
}

void OPTIONSResponse::appendResponseBody(std::string & responseContent)
{
	responseContent += "\r\n";
}
