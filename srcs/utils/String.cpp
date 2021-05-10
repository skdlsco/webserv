#include "String.hpp"

static int		isOverLLMax(const char *str)
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

int		atoi(const char *str)
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
	while (str[idx] && (str[idx] >= '0' && str[idx] <= '9'))
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

int stoi(std::string const & str)
{
	return (atoi(str.c_str()));
}

/* itoa */
static int		getSize(int n)
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

static int		abs(int n)
{
	return (n < 0 ? -n : n);
}

char			*itoa(int val)
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
		result[idx] = '0' + abs(val % 10);
		val /= 10;
		idx--;
	}
	result[size] = '\0';
	return (result);
}

/* toString */
std::string toString(int val)
{
	char *CStr = itoa(val);
	std::string str(CStr);

	/* I don't have certainty below delete code.. */
	delete CStr;
	return (str);
}

/* split */
std::vector<std::string> split(std::string target, std::string token)
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