#include "Connection.hpp"

std::string const Connection::TAG = "Connection";

Connection::Connection(ServerManager &serverManager, std::vector<ServerConfig *> const &config,
						struct sockaddr_in addr, int fd)
: ServerComponent(serverManager), mFDListener(*this), mRequest(config), mCGIResponse(NULL),
	mWriteBuffer(NULL), mConfig(config), mAddr(addr), mFD(fd), mStartTime(web::getNowTime()), mWriteIdx(0),
	mCloseResponseIdx(-1)
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
	delete mWriteBuffer;
	delete mCGIResponse;
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

void Connection::createResponseBuffer()
{
	if (mRequest.getErrorCode())
		mCloseResponseIdx = mResponseVec.size();
	if (mRequest.isCGI())
	{
		mCGIResponse = ResponseFactory::createCGIResponse(getServerManager(), mAddr, mRequest);
		if (mCGIResponse == NULL)
		{
			mWriteBuffer = ResponseFactory::createErrorResponse(mAddr, mRequest, 500);
			if (mWriteBuffer == NULL)
				finish();
		}
	}
	else
	{
		mWriteBuffer = ResponseFactory::create(mAddr, mRequest);
		if (mWriteBuffer == NULL)
			finish();
	}
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

	if (mConnection.mRequest.getAnalyzeLevel() == Request::DONE)
		return ;
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
			mConnection.createResponseBuffer();
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
	if (mConnection.mWriteBuffer)
	{
		int bufferSize = BUFFER_SIZE;

		if (BUFFER_SIZE > mConnection.mWriteBuffer->length() - mConnection.mWriteIdx)
			bufferSize = mConnection.mWriteBuffer->length() - mConnection.mWriteIdx;
		int writeN = write(mConnection.mFD, mConnection.mWriteBuffer->c_str() + mConnection.mWriteIdx, bufferSize);
		if (writeN < 0)
		{
			mConnection.finish();
			return ;
		}
		mConnection.mWriteIdx += writeN;
		if ((mConnection.mWriteBuffer->length() - mConnection.mWriteIdx) <= 0)
			mConnection.finish();
	}
}

void Connection::ConnectionAction::onExceptSet()
{
	mConnection.finish();
}

