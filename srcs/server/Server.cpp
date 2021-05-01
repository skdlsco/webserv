#include "Server.hpp"

Server::Server(ServerManager &serverManager, Config config)
: ServerComponent(serverManager), mConfig(config), mFDListener(*this)
{
	mSocket.bind(mConfig.getPort());
	mSocket.listen(100);
	getServerManager().addFD(mSocket.getSocketFD(), mFDListener);
}

Server *Server::create(ServerManager &serverManager, Config config)
{
	return (new Server(serverManager, config));
}

Server::~Server()
{
	getServerManager().removeFD(mSocket.getSocketFD());
}

const Config &Server::getConfig() const
{
	return (mConfig);
}

void Server::onRepeat()
{

}

Server::ServerAction::ServerAction(Server &server) : mServer(server)
{

}

Server::ServerAction::~ServerAction()
{

}

void Server::ServerAction::onReadSet()
{
	struct sockaddr_in clientAddr;
	int clientFD = mServer.mSocket.accept(clientAddr);
	Connection::create(mServer.getServerManager(), mServer.mConfig, clientAddr, clientFD);
}

void Server::ServerAction::onWriteSet()
{

}

void Server::ServerAction::onExceptSet()
{

}
