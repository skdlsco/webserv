#ifndef SERVER_MANAGER
# define SERVER_MANAGER

class ServerComponent;

# include <algorithm>
# include <string>
# include "fdmanager/FileDiscriptorManager.hpp"
# include "server/ServerComponent.hpp"

class ServerManager
{
	typedef std::vector<ServerComponent *>::iterator componentIter;

	private:
		FileDiscriptorManager mFDManager;
		std::vector<ServerComponent *> mServerComponentVec;

		void resizeComponentVec();
		ServerManager(ServerManager const & copy);
		ServerManager & operator=(ServerManager const & copy);
	public:
		static std::string const TAG;

		ServerManager();
		virtual ~ServerManager();

		void run();
		void addComponent(ServerComponent *component);
		void removeComponent(ServerComponent *component);
		void addFD(int fd, FileDiscriptorListener &fdListener);
		void removeFD(int fd);
		struct timeval makeTimeOut();
};

#endif
