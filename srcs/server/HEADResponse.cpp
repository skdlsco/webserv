#include "HEADResponse.hpp"

std::string const HEADResponse::TAG = "HEADResponse";

HEADResponse::HEADResponse(const ServerConfig * serverConfig,
						const LocationConfig * locationConfig)
: GETResponse(serverConfig, locationConfig)
{

}

HEADResponse::HEADResponse(HEADResponse const & copy)
: GETResponse(copy)
{
	*this = copy;
}

HEADResponse &HEADResponse::operator=(HEADResponse const & rhs)
{
	GETResponse::operator=(rhs);
	return (*this);
}

HEADResponse::~HEADResponse()
{

}

std::string *HEADResponse::getResponse()
{
	std::string responseBody;
	std::string *responseContent;

	try
	{
		responseContent = new std::string();
		if (responseContent)
		{
			setContentLocation();
			responseBody = createResponseBody();

			*responseContent += createResponseLine();
			createResponseHeader(responseBody, *responseContent);

			/* HEAD Method Response don't have body content */
			/* *mResponseContent += createResponseBody(); */
		}

		if (getStatusCode() != 0)
		{
			delete responseContent;
			responseContent = NULL;
		}
		else
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
