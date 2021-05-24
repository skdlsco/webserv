#include "Connection.hpp"

std::string const Connection::TAG = "Connection";

Connection::Connection(ServerManager &serverManager, std::vector<ServerConfig *> const &config,
						struct sockaddr_in addr, int fd)
: ServerComponent(serverManager), mFDListener(*this), mRequest(config), mResponse(NULL),
	mWriteBuffer(NULL), mConfig(config), mAddr(addr), mFD(fd), mStartTime(web::getNowTime())
{
	getServerManager().addFD(fd, mFDListener);
}

Connection::~Connection()
{
	getServerManager().removeFD(mFD);
	delete mResponse;
	delete mWriteBuffer;
}

Connection *Connection::create(ServerManager &serverManager,
							std::vector<ServerConfig *> const &config, struct sockaddr_in addr, int fd)
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
	//getNowTime() - mStartTime > TIMEOUT ? ERROR
	if (mResponse && mResponse->getState() == Response::DONE)
	{
		mWriteBuffer = mResponse->getResponse();
		if (!mWriteBuffer)
			finish();
	}
}

std::vector<ServerConfig *> const &Connection::getConfig() const
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
		if (mConnection.mRequest.getAnalyzeLevel() == Request::DONE)
			mConnection.mResponse = ResponseFactory::create(mConnection.getServerManager(),
															mConnection.mRequest,
															mConnection.mRequest.getConfig());
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
	if (mConnection.mWriteBuffer)
	{
		int bufferSize = BUFFER_SIZE;

		if (BUFFER_SIZE > mConnection.mWriteBuffer->size())
			bufferSize = mConnection.mWriteBuffer->size();
		int writeN = write(mConnection.mFD, mConnection.mWriteBuffer->c_str(), bufferSize);
		if (writeN < 0)
		{
			mConnection.finish();
			return ;
		}
		mConnection.mWriteBuffer->erase(0, writeN);
		if (mConnection.mWriteBuffer->empty())
			mConnection.finish();
	}
}

void Connection::ConnectionAction::onExceptSet()
{
	mConnection.finish();
}

