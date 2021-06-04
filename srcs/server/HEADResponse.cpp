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

void HEADResponse::errorExcept()
{
	mResponseContent = ErrorResponse::getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}

void HEADResponse::run()
{
	initState();
	if (getStatusCode() != 0)
	{
		errorExcept();
		return ;
	}
	try
	{
		std::string responseBody;
		initContentLocation();
		responseBody = appendResponseBody();
		if (getStatusCode() == 0)
		{
			setStatusCode(200);
			setState(DONE);
		}
		appendResponseHeader(responseBody);
		if (getStatusCode() != 200)
		{
			errorExcept();
			return ;
		}
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		setStatusCode(500);
		errorExcept();
	}
}
