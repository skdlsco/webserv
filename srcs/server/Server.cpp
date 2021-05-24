#include "Server.hpp"

std::string const Server::TAG = "Server";

Server::Server(ServerManager &serverManager, std::vector<ServerConfig *> const &config)
: ServerComponent(serverManager), mConfig(config), mFDListener(*this)
{
	mConfig[0]->getPort();
	mSocket.bind(mConfig[0]->getPort());
	mSocket.listen(100);
	getServerManager().addFD(mSocket.getSocketFD(), mFDListener);
	logger::print(TAG) << "listening port = " << mConfig[0]->getPort() << std::endl;
}

Server *Server::create(ServerManager &serverManager,
						std::vector<ServerConfig *> const &config)
{
	try
	{
		return (new Server(serverManager, config));
	}
	catch(std::exception const &e)
	{
		logger::println(TAG, e.what());
	}
	return (NULL);
}

Server::~Server()
{
	getServerManager().removeFD(mSocket.getSocketFD());
}

std::vector<ServerConfig *> const &Server::getConfig() const
{
	return (mConfig);
}

void Server::onRepeat()
{

}

Server::ServerAction::ServerAction(Server &server)
: mServer(server)
{

}

Server::ServerAction::~ServerAction()
{

}

void Server::ServerAction::onReadSet()
{
	struct sockaddr_in clientAddr;
	int clientFD = mServer.mSocket.accept(clientAddr);
	logger::print(TAG) << "accept fd : " << clientFD << std::endl;
	try
	{
		Connection::create(mServer.getServerManager(), mServer.mConfig, clientAddr, clientFD);
	}
	catch(const std::exception& e)
	{
		close(clientFD);
		logger::println(TAG, e.what());
	}
}

void Server::ServerAction::onWriteSet()
{

}

void Server::ServerAction::onExceptSet()
{

}
