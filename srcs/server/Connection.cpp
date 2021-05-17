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
	try
	{
		return (new Connection(serverManager, config, addr, fd));
	}
	catch(std::exception const &e)
	{
		std::cerr << e.what() << '\n';
	}
	return (NULL);
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
	char buffer[BUFFER_SIZE];

	int n = recv(mConnection.mFD, buffer, BUFFER_SIZE - 1, 0);
	if (n == -1)
	{
		mConnection.finish();
		return ;
	}
	buffer[n] = '\0';
	try
	{
		mConnection.mRequest.analyzeBuffer(buffer);
		// if (mConnection.mRequest.getAnalyzeLevel() == DONE)
			// mConnection.mResponse = ResponseFactory.create(mConnection.mRequest);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		mConnection.finish();
		return ;
	}
}

void Connection::ConnectionAction::onWriteSet()
{
	if (mConnection.mRequest.getAnalyzeLevel() == Request::DONE)
	{
		logger::println(TAG, mConnection.mRequest.getTarget());
		char content[] = "HTTP/1.1 200 OK\r\nContent-Length:2\r\n\r\nabc";
		write(mConnection.mFD, content, strlen(content));
		mConnection.finish();
	}
	// response writeBuffer check...
}

void Connection::ConnectionAction::onExceptSet()
{
	mConnection.finish();
}

