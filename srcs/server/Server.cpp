#include "Server.hpp"

const std::string Server::TAG = "Server";

Server::Server(ServerManager &serverManager, Config config)
: ServerComponent(serverManager), mConfig(config), mFDListener(*this)
{
	mSocket.bind(mConfig.getPort());
	mSocket.listen(100);
	getServerManager().addFD(mSocket.getSocketFD(), mFDListener);
	logger::print(TAG) << "listening port = " << config.getPort() << std::endl;
}

Server *Server::create(ServerManager &serverManager, Config config)
{
	try
	{
		return (new Server(serverManager, config));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (NULL);
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
	logger::print(TAG) << "accept fd : " << clientFD << std::endl;
	Connection::create(mServer.getServerManager(), mServer.mConfig, clientAddr, clientFD);
}

void Server::ServerAction::onWriteSet()
{

}

void Server::ServerAction::onExceptSet()
{

}
