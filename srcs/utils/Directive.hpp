#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# define NUM_SERVER_DIRECTIVES 8
# define NUM_LOCATION_DIRECTIVES 6

# include <iostream>
# include <string>

namespace web
{
	std::string serverDirectives[NUM_SERVER_DIRECTIVES] = 
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

	std::string locationDirectives[NUM_LOCATION_DIRECTIVES] = 
	{
		"index",
		"root",
		"autoindex",
		"allow_method",
		"cgi_extension",
		"cgi_path"
	};

	enum ServerDirectives
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

	enum LocationDirectives
	{
		INDEX,
		ROOT,
		AUTOINDEX,
		ALLOW_METHOD,
		CGI_EXTENSION,
		CGI_PATH
	};
}

#endif