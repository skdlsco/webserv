#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ServerComponent.hpp"
# include "fdmanager/FileDiscriptorListener.hpp"
#include "config/Config.hpp"

class Connection : ServerComponent
{
	private:
		class ConnectionAction : public FileDiscriptorListener
		{
			private:
				Connection &mConnection;
				ConnectionAction();
				ConnectionAction(ConnectionAction const & copy);
				ConnectionAction &operator=(ConnectionAction const & copy);
			public:
				ConnectionAction(Connection &connection);
				virtual ~ConnectionAction();

				void onReadSet();
				void onWriteSet();
				void onExceptSet();
		};
	private:
		ConnectionAction mFDListener;
		const Config &mConfig;
		struct sockaddr_in mAddr; // TODO convert type to int, string
		int mFD;

		Connection();
		Connection(ServerManager &serverManager, const Config &config, struct sockaddr_in addr, int fd);
		Connection(Connection const & copy);
		Connection &operator=(Connection const & copy);
	public:
		static const std::string TAG;

		virtual ~Connection();
		static Connection *create(ServerManager &serverManager,
									const Config &config, struct sockaddr_in addr, int fd);

		virtual void onRepeat();
		const Config &getConfig() const;
};

#endif
