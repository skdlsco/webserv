#ifndef RESPONSE_FACTORY_HPP
# define RESPONSE_FACTORY_HPP

#include <iostream>
#include <string>
#include "logger/Logger.hpp"
#include "config/ServerConfig.hpp"
#include "config/LocationConfig.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "server/ErrorResponse.hpp"
#include "server/GETResponse.hpp"
#include "server/POSTResponse.hpp"
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
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		int mStatusCode;
	public:
		static std::string const TAG;

		static std::string *create(Request &request, const ServerConfig *config);
		ResponseFactory(Request &request, const ServerConfig *config);
		virtual ~ResponseFactory();

		Response *createResponse();

		/* 0 or 400(Bad Request) */
		void checkRequestErrorCode();

		/* find specific URI */
		void checkLocationURI();

		/* find method */
		void checkLocationMethodList();

		/* is have CGI? */
		void checkLocationCGI();

		Response *createErrorResponse();
		Response *createCGIResponse();
		Response *createMethodResponse();

		void initResponseValue();

		void setResponseServerConfig(Response *response);
		void setResponseLocationConfig(Response *response);
};

#endif
