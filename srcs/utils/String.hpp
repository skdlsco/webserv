#ifndef STRING_HPP
# define STRING_HPP

#include <iostream>
#include <string>
#include <vector>

namespace web
{
	int atoi(const char *str);
	int stoi(std::string const & str);

	char *itoa(int val);
	std::string toString(int val);

	std::vector<std::string> split(std::string target, std::string token);
};

#endif