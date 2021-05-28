#ifndef STRING_HPP
# define STRING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

namespace web
{
	int atoi(const char *str);
	int axtoi(const char *str);
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

	void addPrefixToString(std::string &target, std::string const prefix);
	void addSuffixToString(std::string &target, std::string const suffix);

	bool isNum(int ch);
	bool isLower(int ch);
	bool isUpper(int ch);
	bool isAlpha(int ch);
	std::string toUpper(std::string &str);
	std::string toLower(std::string &str);
	std::string copyToUpper(std::string const &str);
	std::string copyToLower(std::string const &str);

	std::string removeConsecutiveDuplicate(std::string const str, char c);
};

#endif
