#include "String.hpp"

static int isOverLLMax(const char *str)
{
	const char	LLMax[] = "9223372036854775807";
	int			idx;

	idx = 0;
	while (LLMax[idx])
	{
		if (LLMax[idx] != str[idx])
			return (LLMax[idx] < str[idx]);
		idx++;
	}
	return (0);
}

int web::atoi(const char *str)
{
	int idx = 0;
	int num = 0;
	int sign = 1;

	while (*str && *str == ' ')
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		sign = (*str == '-' ? -1 : 1);
		str++;
	}
	while (str[idx] && web::isNum(str[idx]))
	{
		num *= 10;
		num += str[idx] - '0';
		idx++;
	}
	num *= sign;
	if (idx >= 19 && isOverLLMax(str))
		return ((sign == -1) - 1);
	return (num);
}

int web::axtoi(const char *str)
{
	int idx = 0;
	int num = 0;
	int sign = 1;

	while (*str && *str == ' ')
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		sign = (*str == '-' ? -1 : 1);
		str++;
	}
	while (str[idx] && (web::isNum(str[idx]) ||
			(str[idx] >= 'a' && str[idx] <= 'f') ||
			(str[idx] >= 'A' && str[idx] <= 'F')))
	{
		num *= 16;
		if (web::isNum(str[idx]))
			num += str[idx] - '0';
		if (str[idx] >= 'a' && str[idx] <= 'f')
			num += str[idx] - 'a' + 10;
		if (str[idx] >= 'A' && str[idx] <= 'F')
			num += str[idx] - 'A' + 10;
		idx++;
	}
	num *= sign;
	if (idx >= 19 && isOverLLMax(str))
		return ((sign == -1) - 1);
	return (num);
}

int web::stoi(std::string const & str)
{
	return (atoi(str.c_str()));
}

/* itoa */
static int getSize(int n)
{
	int size;

	if (n == 0)
		return (1);
	size = (n < 0);
	while (n)
	{
		size++;
		n /= 10;
	}
	return (size);
}

static int absolute(int n)
{
	return (n < 0 ? -n : n);
}

char *web::itoa(int val)
{
	int		idx;
	int		size = getSize(val);
	char	*result;

	result = new char[size + 1];
	if (!result)
		return (NULL);

	result[0] = val == 0 ? '0' : '-';
	idx = size - 1;
	while (val)
	{
		result[idx] = '0' + absolute(val % 10);
		val /= 10;
		idx--;
	}
	result[size] = '\0';
	return (result);
}

/* toString */
std::string web::toString(int val)
{
	char *CStr = itoa(val);
	std::string str(CStr);

	/* I don't have certainty below delete code.. */
	delete CStr;
	return (str);
}

/* split */
std::vector<std::string> web::split(std::string target, std::string token)
{
	std::string splitLine;
	std::vector<std::string> splitResult;
	size_t startIdx = 0;
	size_t findTokenIdx = target.find_first_of(token, startIdx);

	while (findTokenIdx != std::string::npos)
	{
		splitLine = target.substr(startIdx, findTokenIdx - startIdx);
		if (splitLine != "")
			splitResult.push_back(splitLine);
		startIdx = findTokenIdx + 1;
		findTokenIdx = target.find_first_of(token, startIdx);
	}
	splitLine = target.substr(startIdx, findTokenIdx - startIdx);
	if (splitLine != "")
		splitResult.push_back(splitLine);

	return (splitResult);
}

/* trim */
void web::ltrim(std::string &s, std::string const set)
{
	s.erase(0, s.find_first_not_of(set));
}

void web::ltrim(std::string &s)
{
	web::ltrim(s, " ");
}

void web::rtrim(std::string &s, std::string const set)
{
	s.erase(s.find_last_not_of(set) + 1);
}

void web::rtrim(std::string &s)
{
	web::rtrim(s, " ");
}

void web::trim(std::string &s, std::string const set)
{
	web::rtrim(s, set);
	web::ltrim(s, set);
}

void web::trim(std::string &s)
{
	web::rtrim(s);
	web::ltrim(s);
}

void web::addPrefixToString(std::string &target, std::string prefix)
{
	target = prefix + target;
}

void web::addSuffixToString(std::string &target, std::string suffix)
{
	target = target + suffix;
}


bool web::isNum(int ch)
{
	if (ch >= '0' && ch <= '9')
		return (true);
	return (false);
}

bool web::isLower(int ch)
{
	if (ch >= 'a' && ch <= 'z')
		return (true);
	return (false);
}

bool web::isUpper(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return (true);
	return (false);
}

bool web::isAlpha(int ch)
{
	if (isLower(ch) || isUpper(ch))
		return (true);
	return (false);
}

std::string web::toUpper(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (web::isLower(str[i]))
			str[i] = str[i] - 'a' + 'A';
	}
	return (str);
}


std::string web::toLower(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (web::isUpper(str[i]))
			str[i] = str[i] - 'A' + 'a';
	}
	return (str);
}

std::string web::copyToUpper(std::string const &str)
{
	std::string upperResult;

	upperResult = str;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (web::isLower(str[i]))
			upperResult[i] = str[i] - 'a' + 'A';
	}
	return (upperResult);
}

std::string web::copyToLower(std::string const &str)
{
	std::string lowerResult;

	lowerResult = str;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (web::isUpper(str[i]))
			lowerResult[i] = str[i] - 'A' + 'a';
	}
	return (lowerResult);
}

std::string web::removeConsecutiveDuplicate(std::string const str, char c)
{
	std::string result;
	char prev = '\0';

	for (std::string::const_iterator iter = str.begin(); iter < str.end(); iter++)
	{
		if (!(prev == c && *iter == c))
			result += *iter;
		prev = *iter;
	}
	return (result);
}
