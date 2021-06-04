#include "Connection.hpp"

std::string const Connection::TAG = "Connection";

Connection::Connection(ServerManager &serverManager, std::vector<ServerConfig *> const &config,
						struct sockaddr_in addr, int fd)
: ServerComponent(serverManager), mFDListener(*this), mRequest(config), 
	mWriteBuffer(NULL), mConfig(config), mAddr(addr), mFD(fd), mStartTime(web::getNowTime()), 
	mWriteIdx(0)
{
	getServerManager().addFD(fd, mFDListener);
}

Connection::~Connection()
{
	getServerManager().removeFD(mFD);
	for (std::vector<Response *>::iterator iter; iter != mResponseVec.end(); iter++)
	{
		delete *iter;
	}
	mResponseVec.clear();
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

void Connection::addResponse()
{
	Response *response = ResponseFactory::create(getServerManager(), mAddr, mRequest);
	if (response == NULL)
	{
		finish();
		return ;
	}
	mResponseVec.push_back(response);
	response->run();
}

void Connection::onRepeat()
{
	/* getNowTime() - mStartTime > TIMEOUT ? ERROR */
	if (web::getNowTime() - mStartTime > TIMEOUT)
		finish();
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

	int n = read(mConnection.mFD, buffer, BUFFER_SIZE - 1);
	if (n <= 0)
	{
		mConnection.finish();
		return ;
	}
	buffer[n] = '\0';
	try
	{
		if (mConnection.mRequest.analyzeBuffer(buffer))
			mConnection.addResponse();
		
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		mConnection.finish();
		return ;
	}
}

void Connection::ConnectionAction::onWriteSet()
{
	if (mConnection.mResponseVec.empty())
		return ;

	Response *response = mConnection.mResponseVec.front();
	if (response == NULL || response->getState() != DONE)
		return ;
	std::string &responseContent = response->getResponse();
	int bufferSize = BUFFER_SIZE;

	if (BUFFER_SIZE > responseContent.length() - mConnection.mWriteIdx)
		bufferSize = responseContent.length() - mConnection.mWriteIdx;
	int writeN = write(mConnection.mFD, responseContent.c_str() + mConnection.mWriteIdx, bufferSize);
	if (writeN < 0)
	{
		mConnection.finish();
		return ;
	}
	mConnection.mWriteIdx += writeN;
	if ((responseContent.length() - mConnection.mWriteIdx) <= 0)
	{
		mConnection.mWriteIdx = 0;
		int isKeepAlive = response->isKeepAlive();
		mConnection.mResponseVec.erase(mConnection.mResponseVec.begin());
		if (!isKeepAlive)
			mConnection.finish();
	}
}

void Connection::ConnectionAction::onExceptSet()
{
	mConnection.finish();
}

