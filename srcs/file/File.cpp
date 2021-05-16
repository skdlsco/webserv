#include "File.hpp"

File::File(std::string const & fileName)
: mState(CONTENT_LEFT), mFileName(fileName)
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
		this->mFileName = rhs.mFileName;
	}
	return (*this);
}

void File::openFile()
{
	mFD = open(mFileName.c_str(), O_RDONLY);
	if (mFD < 0)
		throw File::FileException("file can't open.");
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

File::~File()
{
	mBuffer.clear();
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
