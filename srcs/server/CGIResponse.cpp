#include "CGIResponse.hpp"

std::string const CGIResponse::TAG = "CGIResponse";

CGIResponse::CGIResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig), mEnv(NULL)
{

}

CGIResponse::CGIResponse(CGIResponse const & copy)
: Response(copy)
{
	*this = copy;
}

CGIResponse &CGIResponse::operator=(CGIResponse const & rhs)
{
	Response::operator=(rhs);
	return (*this);
}

CGIResponse::~CGIResponse()
{

}

void CGIResponse::freeEnv(char **env)
{
	for (size_t i = 0; i < 17; i++)
	{
		delete (env[i]);
	}
	delete[] (env);
}

void CGIResponse::initCGIInfo()
{
	std::string target = getTarget();

	for (size_t idx = target.find("."); idx > 0 ; idx--)
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
	mScriptFileName = mDocumentRoot + mScriptName;
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
			result += "";
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
			// if (!getLocationConfig()->getAuthUserName().empty())
				// result += getLocationConfig()->getAuthUserName();
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

char **CGIResponse::createCGIEnv()
{
	bool isError = false;

	mEnv = new char *[NUM_CGI_ENV + 1];
	if (mEnv == NULL)
		return (NULL);
	bzero(mEnv, sizeof(char *) * (NUM_CGI_ENV + 1));
	for (int i = 0; i < NUM_CGI_ENV; i++)
	{
		mEnv[i] = createCGIVariable(static_cast<web::CGIEnv::CGIEnv>(i));
		if (mEnv[i] == NULL)
		{
			isError = true;
			break ;
		}
	}
	for (int i = 0; i < NUM_CGI_ENV; i++)
	{
		logger::println(TAG, mEnv[i]);
	}
	if (isError)
	{
		freeEnv(mEnv);
		mEnv = NULL;
	}
	return (mEnv);
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
	if (execve(getLocationConfig()->getCGIPath().c_str(), argv, mEnv) == -1)
		setStatusCode(500);
	logger::print(TAG) << errno << std::endl;
	free(argv[0]);
	logger::println(TAG, "execve failed");
	exit(1);
}

void CGIResponse::forkCGI()
{
	mPid = fork();
	logger::print(TAG) << "before fork " << getStatusCode() << std::endl;
	if (mPid == -1)
		setStatusCode(500);
	else if (mPid == 0)
		execveCGI();
	logger::print(TAG) << "after fork pid :" << mPid<< std::endl;
	logger::print(TAG) << "after fork " << getStatusCode() << std::endl;
	close(mInPipe[1]);
	close(mOutPipe[0]);
}

void CGIResponse::sendBody()
{
	if (getStatusCode())
		return ;

	std::string requestBody = getRequestBody();
	const char *buffer = requestBody.c_str();
	size_t idx = 0;
	size_t bufferSize = BUFFER_SIZE;
	ssize_t writeN;

	if (bufferSize > requestBody.length() - idx)
		bufferSize =  requestBody.length() - idx;
	while ((writeN = write(mOutPipe[STDOUT_FILENO], buffer + idx, bufferSize)) > 0)
	{
		idx += writeN;
		if (bufferSize > requestBody.length() - idx)
			bufferSize =  requestBody.length() - idx;
	}
	if (writeN == -1)
		setStatusCode(500);
	logger::print(TAG) << idx << std::endl;
	close(mOutPipe[1]);
}

void CGIResponse::readCGI()
{
	if (getStatusCode())
		return ;

	char buffer[BUFFER_SIZE + 1];
	ssize_t readN;

	while ((readN = read(mInPipe[STDIN_FILENO], buffer, BUFFER_SIZE)) > 0)
	{
		buffer[readN] = 0;
		mCGIResponse.append(buffer);
	}
	if (readN == -1)
	{
		setStatusCode(500);
		return ;
	}
}

void CGIResponse::runCGI()
{
	if (pipe(mInPipe) == 0)
	{
		if (pipe(mOutPipe) == 0)
		{
			forkCGI();
			sendBody();
			readCGI();
			close(mOutPipe[0]);
			close(mOutPipe[1]);
			waitpid(mPid, NULL, 0);

		}
		else
			setStatusCode(500);
		close(mInPipe[0]);
		close(mInPipe[1]);
	}
	else
		setStatusCode(500);
}

std::string *CGIResponse::getResponse()
{
	initCGIInfo();
	if (!web::isPathExist(mScriptFileName))
	{
		/* 여기서 체크 해서 404 띄우는게 맞을까 */
		setStatusCode(404);
		return (NULL);
	}
	mEnv = createCGIEnv();
	if (mEnv == NULL)
	{
		setStatusCode(500);
		return (NULL);
	}
	runCGI();
	logger::println(TAG, mCGIResponse);
	// createResponseContent();
	freeEnv(mEnv);
	mEnv = NULL;
	return (NULL);
}

