#include "Connection.hpp"

std::string const Connection::TAG = "Connection";

Connection::Connection(ServerManager &serverManager, const ServerConfig *config, struct sockaddr_in addr, int fd)
: ServerComponent(serverManager), mFDListener(*this), mConfig(config), mAddr(addr), mFD(fd)
{
	getServerManager().addFD(fd, mFDListener);
}

Connection::~Connection()
{
	getServerManager().removeFD(mFD);
}

Connection *Connection::create(ServerManager &serverManager,
							const ServerConfig *config, struct sockaddr_in addr, int fd)
{
	return (new Connection(serverManager, config, addr, fd));
}

void Connection::onRepeat()
{

}

const ServerConfig *Connection::getConfig() const
{
	return (mConfig);
}

Connection::ConnectionAction::ConnectionAction(Connection &connection)
: mConnection(connection)
{

}

Connection::ConnectionAction::~ConnectionAction()
{

}

void Connection::ConnectionAction::onReadSet()
{

}

void Connection::ConnectionAction::onWriteSet()
{
	char content[] = "HTTP/1.1 200 OK\r\nContent-Length:2\r\n\r\nabc";
	write(mConnection.mFD, content, strlen(content));
	mConnection.finish();
}

void Connection::ConnectionAction::onExceptSet()
{

}

