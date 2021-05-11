#ifndef METHOD_HPP
# define METHOD_HPP

# define NUM_METHOD_LIST 6
# include <string>

namespace web
{
	std::string const method[NUM_METHOD_LIST] = 
	{
		"GET",
		"HEAD",
		"PUT",
		"POST",
		"OPTIONS",
		"DELETE"
	};

	enum Method
	{
		GET,
		HEAD,
		PUT,
		POST,
		OPTIONS,
		DELETE
	};
}

#endif