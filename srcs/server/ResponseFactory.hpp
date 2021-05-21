#ifndef RESPONSE_FACTORY_HPP
# define RESPONSE_FACTORY_HPP

#include <iostream>
#include <string>
#include "logger/Logger.hpp"
#include "config/ServerConfig.hpp"
#include "config/LocationConfig.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "utils/String.hpp"
#include "utils/Method.hpp"

class ResponseFactory
{
	enum FactoryState
	{ 
		WORKING, DONE
	};

	private:
		ResponseFactory();
		ResponseFactory(ResponseFactory const & copy);
		ResponseFactory &operator=(ResponseFactory const & rhs);

		enum FactoryState mState;
		Response *mResponse;
		Request &mRequest;
		ServerManager &mServerManager;
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		bool mCGI;
	public:
		static std::string const TAG;
		//request에는 serverManager가 없다. 근데.. Connection에서 호출하니까.. getServerManager로 파라미터로 받아야 할까.
		static Response *Create(ServerManager &serverManager, Request &request, const ServerConfig *config);
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
		void checkTargetCGI();

		void createDetailResponse();
		void createErrorResponse();

		/* set server/location config */
		void setResponseServerConfig(Response *response);
		void setResponseLocationConfig(Response *response);
		void setResponseCGI(Response *response);
};

#endif