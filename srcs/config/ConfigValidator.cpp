#include "ConfigValidator.hpp"

std::string const ConfigValidator::TAG = "ConfigValidator";

ConfigValidator::ConfigValidator(std::string FilePath)
: mFilePath(FilePath)
{

}

ConfigValidator::ConfigValidator(ConfigValidator const & copy)
{
	*this = copy;
}

ConfigValidator &ConfigValidator::operator=(ConfigValidator const & rhs)
{
	if (this != &rhs)
	{
		mFilePath = rhs.mFilePath;
	}
	return (*this);
}

ConfigValidator::~ConfigValidator()
{

}

bool ConfigValidator::isConfigValidate()
{
	if (!isScopeMatched())
		throw ConfigValidator::ConfigValidatorException("The scope order of the config files is out of order.");
	if (!isConfigSequenceMatched())
		throw ConfigValidator::ConfigValidatorException("The directives are out of order in the config file.");
	if (isHostAndPortPairAlreadyExisted())
		throw ConfigValidator::ConfigValidatorException("Two or more server blocks have same server_name, ports, and ip.");
	if (isLocationURIAlreadyExisted())
		throw ConfigValidator::ConfigValidatorException("The config file contains at least two identical URIs.");
	return (true);
}

void ConfigValidator::configSplitByEnter()
{
	std::ifstream configFile;
	std::string line;

	configFile.open(mFilePath);
	if (!configFile.is_open())
		throw ConfigValidator::ConfigValidatorException("The config file not opened.");

	while (getline(configFile, line))
	{
		mEachConfigLine.push_back(line);
	}
	configFile.close();
}

std::vector<std::string> split(std::string target, std::string token)
{
	std::vector<std::string> splitResult;
	std::string splitLine;
	size_t startIdx = 0;
	size_t findTokenIdx = target.find(token);

	while (findTokenIdx != std::string::npos)
	{
		splitLine = target.substr(startIdx, findTokenIdx - startIdx);
		splitResult.push_back(splitLine);
		startIdx = findTokenIdx + 1;
		findTokenIdx = target.find(token);
	}

	return (splitResult);
}


/* scope pair check */
bool ConfigValidator::isScopeMatched()
{
	int		scopeValue = 0;
	std::string currentLine;
	std::vector<std::string> splitResult;

	for (int i = 0; i < mEachConfigLine.size(); i++)
	{
		currentLine = mEachConfigLine[i];
		if (currentLine.find('{') != std::string::npos || 
			currentLine.find('}') != std::string::npos)
		{
			splitResult = split(currentLine, " ");
			if (splitResult.front() == "server" && splitResult.back() == "{" ||
				splitResult.front() == "location" && splitResult.back() == "{")
				scopeValue++;
			else if (splitResult.size() == 1 && splitResult.front() == "}")
				scopeValue--;
			else
				return (false);
		}
	}
	if (scopeValue != 0)
		return (false);
	return (true);
}

bool ConfigValidator::isConfigSequenceMatched()
{
	mLineIndex = 0;
	int countNotFoundDirective = 0;
	std::vector<std::string> splitResult;

	std::string tempLine;
	while (mLineIndex < mEachConfigLine.size())
	{
		/* to find server/location and block end */
		splitResult = split(mEachConfigLine[mLineIndex], " {");

		if (splitResult.front() == "server")
		{
			/* to check server directive */
			mLineIndex++;

			/* loop until finding location block or server end */
			while (mEachConfigLine[mLineIndex].find("location") == std::string::npos &&
					mEachConfigLine[mLineIndex].find("}") == std::string::npos)
			{
				/* set map value zero to initialize */
				initializeCountServerDirective();
				countNotFoundDirective = 0;
				for (size_t idx = 0; idx < NUM_SERVER_DIRECTIVE; idx++)
				{
					countNotFoundDirective++;
					if (mEachConfigLine[mLineIndex].find(web::serverDirective[idx]) != std::string::npos)
						mCountServerDirective[web::serverDirective[idx]]++;
				}

				/* not matched any directive? (to find weird directive) */
				if (countNotFoundDirective == NUM_SERVER_DIRECTIVE)
					return (false);
				mLineIndex++;
			}

			/* is mandatory directive not existed? */
			if (!mCountServerDirective[web::serverDirective[web::ServerDirective::INDEX]] ||
				!mCountServerDirective[web::serverDirective[web::ServerDirective::DEFAULT_ERROR_PAGE]])
				return (false);

			/* is server directive already existed? */
			for (int idx = 0; idx < NUM_SERVER_DIRECTIVE; idx++)
			{
				if (mCountServerDirective[web::serverDirective[idx]] > 1)
					return (false);
			}
		}

		if (splitResult.front() == "location")
		{
			
			while (mEachConfigLine[mLineIndex].find("}") != std::string::npos)
			{
				/* set map value zero to initialize */
				initializeCountLocationDirective();
				countNotFoundDirective = 0;
				for (size_t idx = 0; idx < NUM_LOCATION_DIRECTIVE; idx++)
				{
					countNotFoundDirective++;
					if (mEachConfigLine[mLineIndex].find(web::locationDirective[idx]) != std::string::npos)
						mCountLocationDirective[web::locationDirective[idx]]++;
				}

				/* not found location inner directive? */
				if (countNotFoundDirective == NUM_LOCATION_DIRECTIVE)
					return (false);
				mLineIndex++;
			}

			/* is mandatory directive not existed? */
			if (!mCountLocationDirective[web::locationDirective[web::LocationDirective::INDEX]] ||
				!mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_EXTENSION]] ||
				!mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]])
				return (false);

			/* is server directive already existed? */
			for (size_t idx = 0; idx < NUM_LOCATION_DIRECTIVE; idx++)
			{
				if (mCountLocationDirective[web::locationDirective[idx]] > 1)
					return (false);
			}
		}

		/* to except return false */
		if (splitResult.front() == "}" || splitResult.front() == "")
			mLineIndex++;
		else
			return (false);
	}
	return (true);
}

bool ConfigValidator::isHostAndPortPairAlreadyExisted()
{
	mLineIndex = 0;
	std::string ip;
	std::string port;
	std::string serverName;

	/* I don't have a good idea for below variable names */
	/* key: ip, value: port */
	std::map<std::string, std::string> serverIpPortPair;
	/* key: ip, value: server_name */
	std::map<std::string, std::string> serverIpHostPair;

	while (mLineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[mLineIndex].find("server") != std::string::npos)
		{
			mLineIndex++;

			/* initalize to default value */
			ip = "0.0.0.0";
			port = "8080";
			serverName = ip;

			while (mEachConfigLine[mLineIndex].find("location") == std::string::npos)
			{
				if (mEachConfigLine[mLineIndex].find(web::serverDirective[web::ServerDirective::IP]) != std::string::npos)
					ip = split(mEachConfigLine[mLineIndex], " ").back();
				if (mEachConfigLine[mLineIndex].find(web::serverDirective[web::ServerDirective::PORT]) != std::string::npos)
					port = split(mEachConfigLine[mLineIndex], " ").back();
				if (mEachConfigLine[mLineIndex].find(web::serverDirective[web::ServerDirective::SERVER_NAME]) != std::string::npos)
					serverName = split(mEachConfigLine[mLineIndex], " ").back();
			}
			break;
		}

		for (size_t idx = 0; idx < serverIpPortPair.size(); idx++)
		{
			if (serverIpPortPair.count(ip) && serverIpPortPair[ip] == port && serverIpHostPair[ip] == serverName)
				return (false);
			else
			{
				serverIpPortPair.insert({ip, port});
				serverIpHostPair.insert({ip, serverName});
			}
		}
		mLineIndex++;
	}
}

bool ConfigValidator::isLocationURIAlreadyExisted()
{
	mLineIndex = 0;
	std::string URI;
	std::vector<std::string> URIvector;

	while (mLineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[mLineIndex].find("location") != std::string::npos)
		{
			URI = split(mEachConfigLine[mLineIndex], " {").back();
			for (size_t idx = 0; idx < URIvector.size(); idx++)
			{
				if (URIvector[idx] == URI)
					return (false);
			}
			URIvector.push_back(URI);
		}
	}
	return (true);

}

void ConfigValidator::initializeCountServerDirective()
{
	mCountServerDirective.clear();
	for (size_t i = 0; i < NUM_SERVER_DIRECTIVE; i++)
	{
		mCountServerDirective.insert({web::serverDirective[i], 0});
	}
}

void ConfigValidator::initializeCountLocationDirective()
{
	mCountLocationDirective.clear();
	for (size_t i = 0; i < NUM_LOCATION_DIRECTIVE; i++)
	{
		mCountLocationDirective.insert({web::locationDirective[i], 0});
	}
}

ConfigValidator::ConfigValidatorException::ConfigValidatorException(std::string message) throw()
{
	mMessage = "CONFIG VALIDATOR ERROR: " + message;
}

ConfigValidator::ConfigValidatorException::~ConfigValidatorException() throw()
{

}

const char* ConfigValidator::ConfigValidatorException::what() const throw()
{
	return (mMessage.c_str());
}

