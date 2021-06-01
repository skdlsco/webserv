#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include "ServerManager.hpp"
#include "server/ServerComponent.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "utils/String.hpp"
#include "utils/HTTP.hpp"
#include "logger/Logger.hpp"

class Response
{
	public:
		enum ResponseState
		{
			READY, ON_WORKING, DONE, ERROR
		};
	private:
		int mStatusCode;
		std::string mStatusMessage;
		struct sockaddr_in mClientAddr;
		std::string mTarget;
		std::string mTargetContent;
		std::string mMethod;
		std::string mQuery;
		std::map<std::string, std::string> mRequestHeader;
		std::string mRequestBody;
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		Response();
	protected:
		std::string createResponseLine();
	public:
		static std::string const TAG;
		Response(const ServerConfig * serverConfig, const LocationConfig * locationConfig);
		virtual ~Response();
		Response(Response const & copy);
		Response &operator=(Response const & rhs);

		virtual std::string *getResponse() = 0;

		int getStatusCode() const;
		void setStatusCode(int statusCode);
		struct sockaddr_in getClientAddr() const;
		void setClientAddr(struct sockaddr_in clientAddr);
		std::string getTarget() const;
		void setTarget(std::string target);
		std::string getTargetContent() const;
		void setTargetContent(std::string target);
		std::string getMethod() const;
		void setMethod(std::string method);
		std::string getQuery() const;
		void setQuery(std::string query);
		std::map<std::string, std::string> const &getRequestHeader() const;
		void setRequestHeader(std::map<std::string, std::string> requestHeader);
		std::string getRequestBody() const;
		void setRequestBody(std::string requestBody);
		const ServerConfig *getServerConfig() const;
		void setServerConfig(const ServerConfig *config);
		const LocationConfig *getLocationConfig() const;
		void setLocationConfig(const LocationConfig *config);
};

#endif
