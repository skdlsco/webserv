#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# define FLAG_SERVER 1
# define FLAG_LOCATION 2

# define NUM_COMMON_DIRECTIVE 3
# define NUM_SERVER_DIRECTIVE 8
# define NUM_LOCATION_DIRECTIVE 7

# include <iostream>
# include <string>

namespace web
{
	std::string const commonDirective[NUM_COMMON_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex"
	};

	std::string const serverDirective[NUM_SERVER_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex",
		"ip",
		"port",
		"server_name",
		"client_max_body_size",
		"default_error_page"
	};

	std::string const locationDirective[NUM_LOCATION_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex",
		"allow_method",
		"cgi_extension",
		"cgi_path",
		"auth"
	};

	namespace CommonDirective
	{
		enum List
		{
			INDEX,
			ROOT,
			AUTOINDEX
		};
	}

	namespace ServerDirective
	{
		enum List
		{
			INDEX,
			ROOT,
			AUTOINDEX,
			IP,
			PORT,
			SERVER_NAME,
			CLIENT_MAX_BODY_SIZE,
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
			ALLOW_METHOD,
			CGI_EXTENSION,
			CGI_PATH,
			AUTH
		};
	}
}

#endif