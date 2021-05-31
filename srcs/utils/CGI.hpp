#ifndef CGI_HPP
# define CGI_HPP

#include <string>

#define NUM_CGI_ENV 17

namespace web
{
	namespace CGIEnv
	{
		std::string const cgiEnvName[NUM_CGI_ENV] =
		{
			"AUTH_TYPE",
			"CONTENT_LENGTH",
			"CONTENT_TYPE",
			"GATEWAY_INTERFACE",
			"PATH_INFO",
			"PATH_TRANSLATED",
			"QUERY_STRING",
			"REMOTE_ADDR",
			"REMOTE_IDENT",
			"REMOTE_USER",
			"REQUEST_METHOD",
			"REQUEST_URI",
			"SCRIPT_NAME",
			"SERVER_NAME",
			"SERVER_PORT",
			"SERVER_PROTOCOL",
			"SERVER_SOFTWARE"
		};

		enum CGIEnv
		{
			AUTH_TYPE,
			CONTENT_LENGTH,
			CONTENT_TYPE,
			GATEWAY_INTERFACE,
			PATH_INFO,
			PATH_TRANSLATED,
			QUERY_STRING,
			REMOTE_ADDR,
			REMOTE_IDENT,
			REMOTE_USER,
			REQUEST_METHOD,
			REQUEST_URI,
			SCRIPT_NAME,
			SERVER_NAME,
			SERVER_PORT,
			SERVER_PROTOCOL,
			SERVER_SOFTWARE
		};
	};
};

#endif
