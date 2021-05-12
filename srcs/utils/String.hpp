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

	void ltrim(std::string &s, std::string const set);
	void ltrim(std::string &s);
	void rtrim(std::string &s, std::string const set);
	void rtrim(std::string &s);
	void trim(std::string &s, std::string const set);
	void trim(std::string &s);
};

#endif
