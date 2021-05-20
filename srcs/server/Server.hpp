#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include "server/ServerComponent.hpp"
#include "fdmanager/FileDiscriptorListener.hpp"
#include "socket/Socket.hpp"
#include "config/ServerConfig.hpp"
#include "server/Connection.hpp"
#include "logger/Logger.hpp"

class Server : public ServerComponent
{
	private:
		class ServerAction : public FileDiscriptorListener
		{
			private:
				Server &mServer;
				ServerAction();
				ServerAction(ServerAction const & copy);
				ServerAction &operator=(ServerAction const & copy);
			public:
				ServerAction(Server &mServer);
				virtual ~ServerAction();

				void onReadSet();
				void onWriteSet();
				void onExceptSet();
		};
	private:
		std::vector<ServerConfig *> const &mConfig;
		Socket mSocket;
		ServerAction mFDListener;

		Server();
		Server(Server const & copy);
		Server &operator=(Server const & copy);
		Server(ServerManager &serverManager, std::vector<ServerConfig *> const &config);
	public:
		static const std::string TAG;
		static Server *create(ServerManager &serverManager, std::vector<ServerConfig *> const &config);
		virtual ~Server();

		std::vector<ServerConfig *> const &getConfig() const;
		virtual void onRepeat();
};

#endif
