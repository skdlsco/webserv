#include "File.hpp"

File::File(std::string const & filePath)
: mFD(-1), mState(CONTENT_LEFT), mFilePath(filePath)
{

}

File::File(File const & copy)
{
	*this = copy;
}

File &File::operator=(File const & rhs)
{
	if (this != &rhs)
	{
		this->mState = rhs.mState;
		this->mFD = rhs.mFD;
		this->mFilePath = rhs.mFilePath;
	}
	return (*this);
}

File::~File()
{
	mBuffer.clear();
	closeFile();
}

void File::openFile()
{
	mFD = open(mFilePath.c_str(), O_RDONLY);
	if (mFD < 0)
		throw File::FileException("file name \"" + mFilePath + "\" can't open.");
}

void File::closeFile()
{
	close(mFD);
}

void File::readFile()
{
	std::size_t nRead;
	char readBuffer[BUFFER_SIZE + 1];

	if (!(nRead = read(mFD, readBuffer, BUFFER_SIZE)))
		mState = CONTENT_NOT_LEFT;
	if (nRead < 0)
		throw File::FileException("read error");
	readBuffer[nRead] = '\0';
	mBuffer = mBuffer + readBuffer;
}

std::string File::getLine()
{
	std::string line;
	std::size_t lineIndex;

	if (mState == CONTENT_LEFT && mBuffer.find("\n") == std::string::npos)
		readFile();

	lineIndex = mBuffer.find("\n");
	if (lineIndex != std::string::npos)
	{
		line = mBuffer.substr(0, lineIndex);
		mBuffer.erase(0, lineIndex + 1);
	}
	else if (mState == CONTENT_NOT_LEFT)
	{
		line = mBuffer;
		mBuffer.clear();
		mState = DONE;
	}
	return (line);
}

int File::getFD() const
{
	return (mFD);
}

enum File::State File::getState() const
{
	return (mState);
}

std::string const &File::getFilePath() const
{
	return (mFilePath);
}

std::string const &File::getBuffer() const
{
	return (mBuffer);
}

bool File::isStateDone() const
{
	return (mState == DONE);
}

File::FileException::~FileException() throw()
{

}

File::FileException::FileException(std::string message) throw()
{
	mMessage = "FILE ERROR: " + message;
}

const char *File::FileException::what() const throw()
{
	return (mMessage.c_str());
}

