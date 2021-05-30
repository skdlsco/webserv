#include "CGIResponse.hpp"

std::string const CGIResponse::TAG = "CGIResponse";

CGIResponse::CGIResponse(const ServerConfig * serverConfig, const LocationConfig * locationConfig)
: Response(serverConfig, locationConfig)
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
			target.erase(0, idx + 1);
			size_t scriptIdx = target.find('/');
			if (scriptIdx == target.npos)
				mScriptName = "/" + target;
			else
			{
				mScriptName = "/" + target.substr(0, scriptIdx);
				target.erase(0, scriptIdx);
				mPathInfo = target;
			}
			break ;
		}
	}

	web::removeConsecutiveDuplicate(mDocumentRoot, '/');
	web::removeConsecutiveDuplicate(mScriptName, '/');
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

char **CGIResponse::createCGIEnv()
{
	char **env;
	bool isError = false;

	env = new char *[NUM_CGI_ENV + 1];
	if (env == NULL)
		return (NULL);
	bzero(env, sizeof(char *) * (NUM_CGI_ENV + 1));
	for (int i = 0; i < NUM_CGI_ENV; i++)
	{
		env[i] = createCGIVariable(static_cast<web::CGIEnv::CGIEnv>(i));
		if (env[i] == NULL)
		{
			isError = true;
			break ;
		}
	}
	if (isError)
	{
		freeEnv(env);
		env = NULL;
	}
	return (env);
}

std::string *CGIResponse::getResponse()
{
	initCGIInfo();
	char **env = createCGIEnv();

	if (env == NULL)
	{
		setStatusCode(500);
		return (NULL);
	}
	for (size_t i = 0; env[i] ; i++)
	{
		std::cout << env[i] << std::endl;
	}
	// runCGI
	// pipe setting. // input pipe, output pipe..
	// execveCGI
	// send body
	// read header, body
	// close fds
	// create Response Content
	// return it
	freeEnv(env);
	return (NULL);
}

