#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include "ServerManager.hpp"
#include "server/ServerComponent.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"

class Response : public ServerComponent
{
	public:
		enum ResponseState
		{
			ON_WORKING, DONE, ERROR
		};
	private:
		const ServerConfig *mServerConfig;
		const LocationConfig *mLocationConfig;
		enum ResponseState mState;
		bool mCGIResponse;
	protected:
		virtual std::string createResponseHeader() = 0;
		virtual std::string createResponseBody() = 0;

		void setState(Response::ResponseState state);
	public:
		static std::string const TAG;

		Response(ServerManager &serverManager);
		virtual ~Response();
		Response(Response const & copy);
		Response &operator=(Response const & rhs);

		std::string *getResponse();

		virtual void onRepeat();
		const ServerConfig *getServerConfig() const;
		void setServerConfig(const ServerConfig *config);
		const LocationConfig *getLocationConfig() const;
		void setLocationConfig(const LocationConfig *config);
		Response::ResponseState getState() const;
		const bool isCGIResponse() const;
		void setCGIResponse(const bool CGIResponse);
};

#endif
