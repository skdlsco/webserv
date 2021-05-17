#ifndef SERVER_COMPONENT_HPP
# define SERVER_COMPONENT_HPP

class ServerManager;

#include "ServerManager.hpp"
#include <string>

class ServerComponent
{
	private:
		ServerManager &mServerManager;
		ServerComponent();
		ServerComponent(ServerComponent const & copy);
		ServerComponent &operator=(ServerComponent const & copy);
	public:
		static std::string const TAG;

		ServerComponent(ServerManager &serverManager);
		virtual ~ServerComponent();
		virtual void onRepeat(struct timeval timeOut) = 0;
		void finish();

		ServerManager &getServerManager();
};

#endif
