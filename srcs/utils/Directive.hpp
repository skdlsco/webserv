#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# define NUM_SERVER_DIRECTIVE 8
# define NUM_LOCATION_DIRECTIVE 6

# include <iostream>
# include <string>

namespace web
{
	std::string serverDirective[NUM_SERVER_DIRECTIVE] = 
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

	std::string locationDirective[NUM_LOCATION_DIRECTIVE] = 
	{
		"index",
		"root",
		"autoindex",
		"allow_method",
		"cgi_extension",
		"cgi_path"
	};

	enum ServerDirective
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

	enum LocationDirective
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