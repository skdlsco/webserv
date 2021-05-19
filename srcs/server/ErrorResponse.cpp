#include "ErrorResponse.hpp"

std::string const ErrorResponse::TAG = "ErrorResponse";

ErrorResponse::ErrorResponse(ServerManager &serverManager)
: Response(serverManager), mIsDefault(false), mFile(getServerConfig()->getDefaultErrorPagePath()),
	mFDListener(*this)
{
	try
	{
		mFile.openFile();
		getServerManager().addFD(mFile.getFD(), mFDListener);
	}
	catch(const std::exception& e)
	{
		mIsDefault = true;
		setState(Response::DONE);
	}
}

ErrorResponse::~ErrorResponse()
{
	setErrorToDefault();
}

void ErrorResponse::setErrorToDefault()
{
	if (!mIsDefault)
	{
		mIsDefault = true;
		getServerManager().removeFD(mFile.getFD());
		mFile.closeFile();
		setState(Response::DONE);
	}
}

std::string ErrorResponse::createResponseHeader()
{
	// TODO Header에 넣어야 하는 field들 확인 (Content Length같은 경우는 string.size() + 2로 할 예정 등...)
	return ("");
}

std::string ErrorResponse::createResponseBody()
{
	if (mIsDefault)
	{
		// return static default error
	}
	// else return mFile.getBuffer() + "\r\n";
	return ("");
}

ErrorResponse::ErrorResponseFDListener::ErrorResponseFDListener(ErrorResponse &errorResponse)
: mErrorResponse(errorResponse)
{

}

ErrorResponse::ErrorResponseFDListener::~ErrorResponseFDListener()
{

}

void ErrorResponse::ErrorResponseFDListener::onReadSet()
{
	if (mErrorResponse.mIsDefault)
		return ;
	try
	{
		mErrorResponse.mFile.readFile();
	}
	catch(const std::exception& e)
	{
		mErrorResponse.setErrorToDefault();
	}
}

void ErrorResponse::ErrorResponseFDListener::onWriteSet()
{

}

void ErrorResponse::ErrorResponseFDListener::onExceptSet()
{
	mErrorResponse.setErrorToDefault();
}

