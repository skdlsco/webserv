#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "server/ServerComponent.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "server/CGIResponse.hpp"
#include "server/ResponseFactory.hpp"
#include "fdmanager/FileDiscriptorListener.hpp"
#include "config/ServerConfig.hpp"
#include "logger/Logger.hpp"

class Connection : public ServerComponent
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
		static const int TIMEOUT = 120;
		static const int BUFFER_SIZE = 8192;
		ConnectionAction mFDListener;
		Request mRequest;
		// CGIResponse *mCGIResponse;
		std::string *mWriteBuffer;
		std::vector<ServerConfig *> const &mConfig;
		struct sockaddr_in mAddr; // TODO convert type to int, string
		int mFD;
		long mStartTime;
		int mWriteIdx;
		
		std::vector<Response *> mResponseVec;

		Connection();
		Connection(ServerManager &serverManager, std::vector<ServerConfig *> const &config, struct sockaddr_in addr, int fd);
		Connection(Connection const & copy);
		Connection &operator=(Connection const & copy);
	public:
		static std::string const TAG;

		virtual ~Connection();
		static Connection *create(ServerManager &serverManager,
									std::vector<ServerConfig *> const &config, struct sockaddr_in addr, int fd);

		virtual void onRepeat();
		std::vector<ServerConfig *> const &getConfig() const;
		void addResponse();
};

#endif
