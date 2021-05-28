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
	try
	{
		mResponseContent = new std::string();
		if (mResponseContent)
		{
			*mResponseContent += createResponseLine();
			appendResponseHeader();
			appendResponseBody();
		}
		setStatusCode(200);
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		delete mResponseContent;
		mResponseContent = NULL;
	}
	return (mResponseContent);
}

void OPTIONSResponse::appendResponseHeader()
{
	/* default header */
	*mResponseContent += "Date: " + web::getDate() + "\r\n";
	*mResponseContent += "Server: webserv (chlee, ina)\r\n";
	*mResponseContent += "Connection: close\r\n";

	/* content part */
	*mResponseContent += "Content-Length: 0\r\n";
	*mResponseContent += "Content-Language: en-US\r\n";

	*mResponseContent += "Allow: ";
	for (size_t idx = 0; idx < getLocationConfig()->getAllowMethodList().size(); idx++)
	{
		*mResponseContent += getLocationConfig()->getAllowMethodList()[idx];
		if (idx < getLocationConfig()->getAllowMethodList().size() - 1)
			*mResponseContent += " ";
	}
	*mResponseContent += "\r\n";
}

void OPTIONSResponse::appendResponseBody()
{
	*mResponseContent += "\r\n";
}
