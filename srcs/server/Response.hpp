#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include "ServerManager.hpp"
#include "server/ServerComponent.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "utils/String.hpp"
#include "utils/HTTP.hpp"
#include "logger/Logger.hpp"

class Response : public ServerComponent
{
	public:
		enum ResponseState
		{
			READY, ON_WORKING, DONE, ERROR
		};
	private:
		int mStatusCode;
		std::string mStatusMessage;
		std::string mTarget;
		std::map<std::string, std::string> mRequestHeader;
		std::string mRequestBody;
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		enum ResponseState mState;
		Response();
	protected:
		virtual std::string createResponseLine();
		virtual void run() = 0;
		virtual std::string createResponseHeader() = 0;
		virtual std::string createResponseBody() = 0;

		void setState(Response::ResponseState state);
	public:
		static std::string const TAG;
		Response(ServerManager &serverManager, const ServerConfig * serverConfig,
					const LocationConfig * locationConfig);
		virtual ~Response();
		Response(Response const & copy);
		Response &operator=(Response const & rhs);

		void start();
		std::string *getResponse();

		virtual void onRepeat();
		int getStatusCode() const;
		void setStatusCode(int statusCode);
		std::string getTarget() const;
		void setTarget(std::string target);
		std::map<std::string, std::string> getRequestHeader() const;
		void setRequestHeader(std::map<std::string, std::string> requestHeader);
		std::string getRequestBody() const;
		void setRequestBody(std::string requestBody);
		const ServerConfig *getServerConfig() const;
		void setServerConfig(const ServerConfig *config);
		const LocationConfig *getLocationConfig() const;
		void setLocationConfig(const LocationConfig *config);
		Response::ResponseState getState() const;
};

#endif
