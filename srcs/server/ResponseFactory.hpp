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

		ServerManager &mServerManager;
		enum ResponseType mResponseState;
		Response *mResponse;
		Request &mRequest;
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		int mStatusCode;
	public:
		static std::string const TAG;
		static Response *create(ServerManager &serverManager, Request &request, const ServerConfig *config);
		ResponseFactory(ServerManager &serverManager, Request &request, const ServerConfig *config);
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

		void createErrorResponse();
		void createCGIResponse();
		void createMethodResponse();

		void setResponseServerConfig(Response *response);
		void setResponseLocationConfig(Response *response);
};

#endif
