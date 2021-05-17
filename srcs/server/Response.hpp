#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include "ServerManager.hpp"
#include "server/ServerComponent.hpp"
#include "config/LocationConfig.hpp"
class Response : public ServerComponent
{
	public:
		enum ResponseState
		{
			ON_WORKING, DONE, ERROR
		};
	private:
		const LocationConfig *mConfig;
		enum ResponseState mState;
	protected:
		virtual std::string createResponseHeader() = 0;
		virtual std::string createResponseBody() = 0;
	public:
		static std::string const TAG;

		Response(ServerManager &serverManager);
		virtual ~Response();
		Response(Response const & copy);
		Response &operator=(Response const & rhs);

		std::string *getResponse();

		virtual void onRepeat();
		void setConfig(const LocationConfig *config) const;
		const LocationConfig *getConfig() const;
		Response::ResponseState getState() const;
};

#endif
