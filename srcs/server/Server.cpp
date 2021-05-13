#include "Server.hpp"

std::string const Server::TAG = "Server";

Server::Server(ServerManager &serverManager, const ServerConfig *config)
: ServerComponent(serverManager), mConfig(config), mFDListener(*this)
{
	mSocket.bind(mConfig->getPort());
	mSocket.listen(100);
	getServerManager().addFD(mSocket.getSocketFD(), mFDListener);
	logger::print(TAG) << "listening port = " << mConfig->getPort() << std::endl;
}

Server *Server::create(ServerManager &serverManager, const ServerConfig *config)
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

const ServerConfig *Server::getConfig() const
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
	Connection::create(mServer.getServerManager(), mServer.mConfig, clientAddr, clientFD);
}

void Server::ServerAction::onWriteSet()
{

}

void Server::ServerAction::onExceptSet()
{

}
