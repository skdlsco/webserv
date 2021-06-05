#include "CGIResponse.hpp"

std::string const CGIResponse::TAG = "CGIResponse";

CGIResponse::CGIResponse(ServerManager &serverManager, const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mServerManager(serverManager), mWriteIdx(0), mIsCGIReadHeader(true),
	mEnv(NULL), mPid(-1), mWriteListener(*this), mReadListener(*this)
{
	mInPipe[0] = -1;
	mInPipe[1] = -1;
	mOutPipe[0] = -1;
	mOutPipe[1] = -1;
}

CGIResponse::CGIResponse(CGIResponse const & copy)
: Response(copy), mServerManager(copy.mServerManager), mWriteListener(*this), mReadListener(*this)
{
	*this = copy;
}

CGIResponse &CGIResponse::operator=(CGIResponse const & rhs)
{
	Response::operator=(rhs); // how...
	return (*this);
}

CGIResponse::~CGIResponse()
{
	freeEnv(mEnv);
	mEnv = NULL;
	if (mInPipe[0] != -1)
		mServerManager.removeFD(mInPipe[0]);
	close(mInPipe[0]);
	close(mInPipe[1]);
	close(mOutPipe[0]);
	if (mOutPipe[1] != -1)
		mServerManager.removeFD(mOutPipe[1]);
	close(mOutPipe[0]);
	close(mOutPipe[1]);
}

void CGIResponse::errorExcept()
{
	if (mPid != -1)
		kill(mPid, SIGKILL);
	mPid = -1;
	mResponseContent = ErrorResponse::getErrorResponse(getServerConfig(), getLocationConfig(), getStatusCode());
	setState(DONE);
}


void CGIResponse::freeEnv(char **env)
{
	if (env == NULL)
		return ;
	for (size_t i = 0; env[i]; i++)
	{
		delete (env[i]);
	}
	delete[] (env);
}

void CGIResponse::initCGIInfo()
{
	std::string target = getTargetContent();

	for (size_t idx = target.find("."); idx >= 0 ; idx--)
	{
		if (target[idx] == '/')
		{
			mDocumentRoot = getLocationConfig()->getRoot() + target.substr(0, idx);
			mDocumentRoot = web::removeConsecutiveDuplicate(mDocumentRoot, '/');
			target.erase(0, idx + 1);
			size_t scriptIdx = target.find('/');
			if (scriptIdx == target.npos)
				mScriptName = web::removeConsecutiveDuplicate("/" + target, '/');
			else
			{
				mScriptName = web::removeConsecutiveDuplicate("/" + target.substr(0, scriptIdx), '/');
				target.erase(0, scriptIdx);
				mPathInfo = target;
			}
			break ;
		}
	}
	mScriptFileName = web::removeConsecutiveDuplicate(mDocumentRoot + mScriptName, '/');
}

std::string CGIResponse::getCGIVariableContentType()
{
	std::map<std::string, std::string>::const_iterator iter =
				getRequestHeader().find("CONTENT-TYPE");

	if (iter != getRequestHeader().end())
		return (iter->second);
	return ("");
}

std::string CGIResponse::getCGIVariableUserAgent()
{
	std::map<std::string, std::string>::const_iterator iter =
				getRequestHeader().find("USER-AGENT");

	if (iter != getRequestHeader().end())
		return (iter->second);
	return ("");
}

char *CGIResponse::createCGIVariable(enum web::CGIEnv::CGIEnv cgiEnv)
{
	std::string result = web::CGIEnv::cgiEnvName[cgiEnv] + "=";

	switch (cgiEnv)
	{
		case web::CGIEnv::AUTH_TYPE:
			if (!getLocationConfig()->getAuthUserName().empty())
				result += "Basic";
			break ;
		case web::CGIEnv::CONTENT_LENGTH:
			if (!getRequestBody().empty())
				result += web::toString(getRequestBody().length());
			break ;
		case web::CGIEnv::CONTENT_TYPE:
			result += getCGIVariableContentType();
			break ;
		case web::CGIEnv::GATEWAY_INTERFACE:
			result += "CGI/1.1";
			break ;
		case web::CGIEnv::PATH_INFO:
			if (mPathInfo.empty())
				result += getTarget();
			else
				result += mPathInfo;
			break ;
		case web::CGIEnv::PATH_TRANSLATED:
			result += web::removeConsecutiveDuplicate(mDocumentRoot + mPathInfo, '/');
			break ;
		case web::CGIEnv::QUERY_STRING:
			result += getQuery();
			break ;
		case web::CGIEnv::REMOTE_ADDR:
			result += web::toAddr(getClientAddr().sin_addr.s_addr);
			break ;
		case web::CGIEnv::REMOTE_IDENT:
			result += getCGIVariableUserAgent();
			break ;
		case web::CGIEnv::REMOTE_USER:
			if (!getLocationConfig()->getAuthUserName().empty())
				result += getLocationConfig()->getAuthUserName();
			break ;
		case web::CGIEnv::REQUEST_METHOD:
			result += getMethod();
			break ;
		case web::CGIEnv::REQUEST_URI:
			result += getTarget();
			break ;
		case web::CGIEnv::SCRIPT_NAME:
			result += mScriptName;
			break ;
		case web::CGIEnv::SERVER_NAME:
			result += getServerConfig()->getServerName();
			break ;
		case web::CGIEnv::SERVER_PORT:
			result += web::toString(getServerConfig()->getPort());
			break ;
		case web::CGIEnv::SERVER_PROTOCOL:
			result += "HTTP/1.1";
			break ;
		case web::CGIEnv::SERVER_SOFTWARE:
			result += "webserv (chlee, ina)";
			break ;
	}
	return (strdup(result.c_str()));
}

std::string CGIResponse::dashToUnderBar(std::string str)
{
	for (size_t idx = 0; idx < str.length(); idx++)
	{
		if (str[idx] == '-')
		str[idx] = '_';
	}
	return (str);
}

char **CGIResponse::createCGIEnv()
{
	char **env;;
	std::vector<std::string> customHeaderVec;

	for (std::map<std::string, std::string>::const_iterator iter = getRequestHeader().begin(); iter != getRequestHeader().end(); iter++)
	{
		if (iter->first.find("X-") == 0)
			customHeaderVec.push_back("HTTP_" + dashToUnderBar(iter->first) + "=" +iter->second);
	}
	int envSize = NUM_CGI_ENV + customHeaderVec.size();
	env = new char *[envSize + 1];
	if (env == NULL)
		return (NULL);
	bzero(env, sizeof(char *) * (envSize + 1));
	for (int idx = 0; idx < NUM_CGI_ENV; idx++)
	{
		env[idx] = createCGIVariable(static_cast<web::CGIEnv::CGIEnv>(idx));
		if (env[idx] == NULL)
		{
			freeEnv(env);
			return (NULL);
		}
	}
	std::vector<std::string>::iterator iter = customHeaderVec.begin();
	for (int idx = NUM_CGI_ENV; idx < envSize; idx++, iter++)
	{
		try
		{
			env[idx] = strdup((*iter).c_str());
		}
		catch(const std::exception& e)
		{
			logger::println(TAG, e.what());
		}
		if (env[idx] == NULL)
		{
			freeEnv(env);
			return (NULL);
		}
	}
	return (env);
}

void CGIResponse::execveCGI()
{
	char *argv[2];

	argv[0] = strdup(getLocationConfig()->getCGIPath().c_str());
	if (argv[0] == NULL)
	{
		setStatusCode(500);
		exit(1);
	}
	close(mOutPipe[1]);
	close(mInPipe[0]);
	dup2(mOutPipe[0], STDIN_FILENO);
	dup2(mInPipe[1], STDOUT_FILENO);
	argv[1] = NULL;
	execve(getLocationConfig()->getCGIPath().c_str(), argv, mEnv);
	free(argv[0]);
	exit(1);
}

void CGIResponse::forkCGI()
{
	mPid = fork();
	if (mPid == -1)
	{
		setStatusCode(500);
		errorExcept();
		return ;
	}
	else if (mPid == 0)
		execveCGI();
	close(mInPipe[1]);
	mInPipe[1] = -1;
	close(mOutPipe[0]);
	mOutPipe[0] = -1;
}

void CGIResponse::sendBody()
{
	if (getState() == DONE || (mRequestBody.length() - mWriteIdx) <= 0)
		return ;

	const char *buffer = mRequestBody.c_str();
	size_t bufferSize = BUFFER_SIZE;

	if (bufferSize > mRequestBody.length() - mWriteIdx)
		bufferSize =  mRequestBody.length() - mWriteIdx;
	ssize_t writeN = write(mOutPipe[STDOUT_FILENO], buffer + mWriteIdx, bufferSize);
	if (writeN == -1)
	{
		setStatusCode(500);
		errorExcept();
		return ;
	}
	mWriteIdx += writeN;
}

bool CGIResponse::responseToHeader()
{
	std::string line;
	size_t lineIdx = mCGIResponse.find("\r\n");

	while (lineIdx != std::string::npos)
	{
		line = mCGIResponse.substr(0, lineIdx);
		mCGIResponse.erase(0, lineIdx + 2);
		if (line.empty())
			return (false);
		web::trim(line);
		size_t coloneIdx = line.find(":");
		std::string key = line.substr(0, coloneIdx);
		std::string value = line.substr(coloneIdx + 1);
		web::trim(value);
		mResponseHeader.insert(std::pair<std::string, std::string>(key, value));
		lineIdx = mCGIResponse.find("\r\n");
	}
	return (true);
}

void CGIResponse::readCGI()
{
	if (getState() == DONE)
		return ;
	char buffer[BUFFER_SIZE + 1];
	ssize_t readN;

	readN = read(mInPipe[STDIN_FILENO], buffer, BUFFER_SIZE);
	if (readN == -1)
	{
		setStatusCode(500);
		errorExcept();
		return ;
	}
	buffer[readN] = 0;
	mCGIResponse.append(buffer);
	if (mIsCGIReadHeader)
		mIsCGIReadHeader = responseToHeader();
	if (readN == 0)
	{
		mState = DONE;
		mServerManager.removeFD(mInPipe[0]);
		mInPipe[0] = -1;
		mServerManager.removeFD(mOutPipe[1]);
		mOutPipe[1] = -1;
		createResponseContent();
	}
}

void CGIResponse::runCGI()
{
	if (pipe(mInPipe) == 0)
	{
		if (pipe(mOutPipe) == 0)
		{
			forkCGI();
			if (mState == DONE)
				return ;
			mRequestBody = getRequestBody();
			mServerManager.addFD(mInPipe[0], mReadListener);
			mServerManager.addFD(mOutPipe[1], mWriteListener);
			return ;
		}
	}
	errorExcept();
}

void CGIResponse::appendResponseHeader()
{
	std::map<std::string, std::string>::iterator iter;

	iter = mResponseHeader.find("Status");
	if (iter == mResponseHeader.end())
		throw std::exception();
	mResponseContent += "HTTP/1.1 " + iter->second + "\r\n";
	mResponseHeader.erase("Status");
	mResponseContent += createDefaultResponseHeader();
	iter = mResponseHeader.find("Content-Length");
	if (iter == mResponseHeader.end())
		mResponseContent += "Content-Length: " + web::toString(mCGIResponse.length()) + "\r\n";
	for (iter = mResponseHeader.begin(); iter != mResponseHeader.end(); iter++)
	{
		mResponseContent += iter->first + ": " + iter->second + "\r\n";
	}
	mResponseContent += "\r\n";
}

void CGIResponse::createResponseContent()
{
	try {
		appendResponseHeader();
		mResponseContent += mCGIResponse;
	}
	catch(const std::exception& e)
	{
		logger::println(TAG, e.what());
		errorExcept();
	}
}

void CGIResponse::checkAuthorization()
{
	if (getLocationConfig()->getAuthUserName().empty() || getLocationConfig()->getAuthUserPassword().empty())
		return ;
	std::map<std::string, std::string> requestHeader = getRequestHeader();
	if (requestHeader.find("Authorization") == requestHeader.end())
	{
		/* Unauthorized */
		setStatusCode(401);
		return ;
	}
	std::string userAuth = web::base64Decoder(requestHeader["Authorization"]);
	std::string serverAuth = getLocationConfig()->getAuthUserName() + ":" + getLocationConfig()->getAuthUserPassword();
	if (userAuth != serverAuth)
	{
		/* Forbidden */
		setStatusCode(403);
		return ;
	}
}

void CGIResponse::run()
{
	checkAuthorization();
	if (getStatusCode())
	{
		errorExcept();
		return ;
	}
	initCGIInfo();
	mEnv = createCGIEnv();
	if (mEnv == NULL)
	{
		setStatusCode(500);
		errorExcept();
		return ;
	}
	runCGI();
}

CGIResponse::ReadListener::ReadListener(CGIResponse &response)
: mResponse(response)
{

}

CGIResponse::ReadListener::~ReadListener()
{

}

void CGIResponse::ReadListener::onReadSet()
{
	mResponse.readCGI();
}

void CGIResponse::ReadListener::onWriteSet() { }

void CGIResponse::ReadListener::onExceptSet() { }

CGIResponse::WriteListener::WriteListener(CGIResponse &response)
: mResponse(response)
{

}

CGIResponse::WriteListener::~WriteListener()
{

}

void CGIResponse::WriteListener::onReadSet() { }

void CGIResponse::WriteListener::onWriteSet() {
	mResponse.sendBody();
}

void CGIResponse::WriteListener::onExceptSet() { }
