#ifndef RESPONSE_FACTORY_HPP
# define RESPONSE_FACTORY_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include "logger/Logger.hpp"
#include "config/ServerConfig.hpp"
#include "config/LocationConfig.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "server/ErrorResponse.hpp"
#include "server/CGIResponse.hpp"
#include "server/GETResponse.hpp"
#include "server/HEADResponse.hpp"
#include "server/POSTResponse.hpp"
#include "server/PUTResponse.hpp"
#include "server/OPTIONSResponse.hpp"
#include "server/DELETEResponse.hpp"
#include "utils/String.hpp"
#include "utils/Method.hpp"

class ResponseFactory
{
	enum ResponseType
	{
		ERROR, CGI, METHOD
	};

	private:
		ResponseFactory();
		ResponseFactory(ResponseFactory const & copy);
		ResponseFactory &operator=(ResponseFactory const & rhs);

		enum ResponseType mResponseState;
		Request &mRequest;
		struct sockaddr_in mClientAddr;
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		int mStatusCode;
	public:
		static std::string const TAG;

		static std::string *create(struct sockaddr_in clientAddr,
												Request &request);
		static std::string *createTimeoutResponse(struct sockaddr_in clientAddr,
													Request &request);
		ResponseFactory(struct sockaddr_in clientAddr, Request &request);
		virtual ~ResponseFactory();

		Response *createResponse();

		/* check response type with request info */
		void checkResponseType();

		Response *createErrorResponse();
		Response *createCGIResponse();
		Response *createMethodResponse();

		void initResponseValue();

		void setResponseServerConfig(Response *response);
		void setResponseLocationConfig(Response *response);
};

#endif
