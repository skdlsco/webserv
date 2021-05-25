#include "POSTResponse.hpp"

std::string const POSTResponse::TAG = "POST Response";

// uri 체크 (폴더는 추가가 안되도록 한다?..)
// naming -> target_ms.target_type;?
// auth check -> uri check -> fd open(naming), write body -> done -> 201(create) (Location, Content-Location)

POSTResponse::POSTResponse(ServerManager &serverManager, const ServerConfig * serverConfig,
				const LocationConfig * locationConfig)
: Response(serverManager, serverConfig, locationConfig), mFD(-1), mFDListener(*this), mBody(getRequestBody())
{
	try
	{
		checkAuthorization();
		checkTarget();
		openFile();
	}
	catch(const std::exception& e)
	{
		setError(500);
		logger::println(TAG, e.what());
	}
}

POSTResponse::~POSTResponse()
{
	getServerManager().removeFD(mFD);
}

POSTResponse::POSTResponse(POSTResponse const & copy)
: Response(copy), mFDListener(*this), mFD(-1)
{
	*this = copy;
}

POSTResponse &POSTResponse::operator=(POSTResponse const & rhs)
{
	Response::operator=(rhs);
	return (*this);
}

void POSTResponse::setError(int errorCode)
{
	setStatusCode(errorCode);
	setState(Response::ERROR);
}

void POSTResponse::checkAuthorization()
{
	if (getLocationConfig()->getAuthUserName().empty() || getLocationConfig()->getAuthUserPassword().empty())
		return ;
	std::map<std::string, std::string> requestHeader = getRequestHeader();
	if (requestHeader.find("Authorization") == requestHeader.end())
	{
		/* Unauthorized */
		setError(401);
		return ;
	}
	std::string userAuth = web::base64Decoder(requestHeader["Authorization"]);
	std::string serverAuth = getLocationConfig()->getAuthUserName() + ":" + getLocationConfig()->getAuthUserPassword();
	if (userAuth != serverAuth)
	{
		/* Forbidden */
		setError(403);
		return ;
	}
}

bool POSTResponse::isFolderExist(std::string dir)
{
	struct stat buf;

	if (stat(dir.c_str(), &buf) == -1)
		return (false);
	return (buf.st_mode & S_IFDIR);
}

bool POSTResponse::isFileExist(std::string dir)
{
	struct stat buf;

	return (stat(dir.c_str(), &buf) == 0);
}

void POSTResponse::createFileName(std::string path)
{
	if (!isFileExist(path))
	{
		mFileName = path;
		return ;
	}
	int number = 1;
	while (number > 0)
	{
		std::string fileName = path + " (" + web::toString(number) + ")";
		if (!isFileExist(path))
		{
			mFileName = fileName;
			return ;
		}
		number++;
	}
}

void POSTResponse::checkTarget()
{
	if (getState() == Response::ERROR)
		return ;
	std::string path = getLocationConfig()->getRoot() + getTarget();

	path = web::removeConsecutiveDuplicate(path, '/');
	int slashIdx = path.find_last_of("/");

	std::string folder = path.substr(0, slashIdx);
	std::string file = path.substr(slashIdx + 1);

	/* 폴더인 경우 404(이부분은 고민이 되네요), 경로가 존재하지 않는 경우 404 */
	if (file.empty() || !isFolderExist(folder))
	{
		setError(404);
		return ;
	}
	createFileName(path);
}

void POSTResponse::openFile()
{
	mFD = open(mFileName.c_str(), O_CREAT | O_WRONLY);
	if (mFD == -1)
	{
		/* 403으로 처리?*/
		setError(403);
		return ;
	}
	getServerManager().addFD(mFD, mFDListener);
}

void POSTResponse::onRepeat()
{

}

std::string POSTResponse::createResponseHeader()
{
	std::string responseHeader;

	responseHeader += "Date: " + web::getDate() + "\r\n";
	responseHeader += "Server: webserv (chlee, ina)\r\n";
	responseHeader += "Connection: close\r\n";

	std::string location = mFileName;
	size_t pos = location.find(getLocationConfig()->getRoot());
	location.erase(pos, getLocationConfig()->getRoot().length());

	responseHeader += "Location: " + location +"\r\n";
	responseHeader += "Content-Location: " + location +"\r\n";
	responseHeader += "\r\n";
	return (responseHeader);
}

std::string POSTResponse::createResponseBody()
{
	return ("\r\n");
}


POSTResponse::POSTResponseFDListener::POSTResponseFDListener(POSTResponse &response)
: mResponse(response)
{

}

POSTResponse::POSTResponseFDListener::~POSTResponseFDListener()
{

}

void POSTResponse::POSTResponseFDListener::onReadSet()
{

}

void POSTResponse::POSTResponseFDListener::onWriteSet()
{
	int writeN = write(mResponse.mFD, mResponse.mBody.c_str(), BUFFER_SIZE);

	if (writeN == -1)
	{
		/* server error */
		mResponse.setError(500);
		return ;
	}
	mResponse.mBody.erase(0, writeN);
	if (mResponse.mBody.empty())
	{
		mResponse.setStatusCode(201);
		mResponse.setState(Response::DONE);
	}
}

void POSTResponse::POSTResponseFDListener::onExceptSet()
{
	/* Error code 무엇으로 하는게 맞는가 */
	mResponse.setError(500);
}
