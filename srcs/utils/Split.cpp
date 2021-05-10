#include "Split.hpp"

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
