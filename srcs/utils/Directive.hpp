#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# define FLAG_SERVER 1
# define FLAG_LOCATION 2

# define NUM_COMMON_DIRECTIVE 4
# define NUM_SERVER_DIRECTIVE 8
# define NUM_LOCATION_DIRECTIVE 9

# include <iostream>
# include <string>

namespace web
{
	std::string const commonDirective[NUM_COMMON_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex",
		"client_max_body_size"
	};

	std::string const serverDirective[NUM_SERVER_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex",
		"client_max_body_size",
		"ip",
		"port",
		"server_name",
		"default_error_page"
	};

	std::string const locationDirective[NUM_LOCATION_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex",
		"client_max_body_size",
		"allow_method",
		"cgi_extension",
		"cgi_path",
		"cgi_method",
		"auth"
	};

	namespace CommonDirective
	{
		enum List
		{
			INDEX,
			ROOT,
			AUTOINDEX,
			CLIENT_MAX_BODY_SIZE
		};
	}

	namespace ServerDirective
	{
		enum List
		{
			INDEX,
			ROOT,
			AUTOINDEX,
			CLIENT_MAX_BODY_SIZE,
			IP,
			PORT,
			SERVER_NAME,
			DEFAULT_ERROR_PAGE
		};
	}

	namespace LocationDirective
	{
		enum List
		{
			INDEX,
			ROOT,
			AUTOINDEX,
			CLIENT_MAX_BODY_SIZE,
			ALLOW_METHOD,
			CGI_EXTENSION,
			CGI_PATH,
			CGI_METHOD,
			AUTH
		};
	}
}

#endif