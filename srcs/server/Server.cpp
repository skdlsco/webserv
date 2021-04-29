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
	int clientfd = mServer.mSocket.accept(clientAddr);
	std::cout << "client fd = " << clientfd << std::endl;
	char content[] = "HTTP/1.1 200 OK\r\nContent-Length:2\r\n\r\nabc";
	write(clientfd, content, strlen(content));
	close(clientfd);
}

void Server::ServerAction::onWriteSet()
{

}

void Server::ServerAction::onExceptSet()
{

}
