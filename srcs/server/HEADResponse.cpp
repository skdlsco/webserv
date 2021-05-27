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

	try
	{
		mResponseContent = new std::string();
		if (mResponseContent)
		{
			setContentLocation();
			responseBody = createResponseBody();

			*mResponseContent += createResponseLine();
			createResponseHeader(responseBody);

			/* HEAD Method Response don't have body content */
			/* *mResponseContent += createResponseBody(); */
		}

		if (getStatusCode() != 0)
		{
			delete mResponseContent;
			mResponseContent = NULL;
		}
		else
			setStatusCode(200);
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		delete mResponseContent;
		mResponseContent = NULL;
	}
	return (mResponseContent);
}
