#include "ConfigValidator.hpp"

std::string const ConfigValidator::TAG = "ConfigValidator";

ConfigValidator::ConfigValidator(std::string const & FilePath)
: mFilePath(FilePath)
{
	readConfigFileByLine();
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
		mEachConfigLine = rhs.mEachConfigLine;
	}
	return (*this);
}

ConfigValidator::~ConfigValidator()
{

}


bool ConfigValidator::isConfigValidate()
{
	/* first, scope pair check. */
	if (!isScopeMatched())
		throw ConfigValidator::ConfigValidatorException("The scope order of the config files is out of order.");
	/* second, check config info sequence. */
	if (!isConfigSequenceMatched())
		throw ConfigValidator::ConfigValidatorException("The directives are out of order in the config file.");
	/* third, */
	if (isServerInfoAlreadyExisted())
		throw ConfigValidator::ConfigValidatorException("Two or more server blocks have same server_name, ports, and ip.");
	if (isLocationURIAlreadyExisted())
		throw ConfigValidator::ConfigValidatorException("The config file contains at least two identical URIs.");
	return (true);
}

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
			splitResult = web::split(currentLine, " ");
			if ((splitResult.size() == 2 && (splitResult.front() == "server" && splitResult.back() == "{")) || //server {
				(splitResult.size() == 3 && (splitResult.front() == "location" && splitResult.back() == "{"))) //location [URI] {
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
	size_t lineIndex = 0;
	std::vector<std::string> splitResult;

	while (lineIndex < mEachConfigLine.size())
	{
		/* to find server/location and block end */
		splitResult = web::split(mEachConfigLine[lineIndex], " {");

		if (splitResult.front() == "server")
		{
			/* to avoid out of bound error */
			if (lineIndex + 1 < mEachConfigLine.size())
				lineIndex++;

			if (!isValidateServerDirective(lineIndex))
				return (false);
			if (!hasMandatoryDirective(FLAG_SERVER) || !hasEachDirectiveOnlyOne(FLAG_SERVER))
				return (false);
		}

		if (splitResult.front() == "location")
		{
			if (!isValidateLocationDirective(lineIndex))
				return (false);
			if (!hasMandatoryDirective(FLAG_LOCATION) || !hasEachDirectiveOnlyOne(FLAG_LOCATION))
				return (false);
		}

		if (splitResult.front() == "}")
			lineIndex++;
		else
			return (false);
	}
	return (true);
}

bool ConfigValidator::isServerInfoAlreadyExisted()
{
	size_t lineIndex = 0;
	std::string ip;
	std::string port;
	std::string serverName;

	/* I don't have a good idea for below variable names */
	std::string serverInfoLine;
	std::vector<std::string> serverInfoList;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("server") != std::string::npos)
		{
			/* to avoid out of bound error */
			if (lineIndex + 1 < mEachConfigLine.size())
				lineIndex++;

			/* initalize to default value */
			ip = "0.0.0.0";
			port = "8080";
			serverName = ip;

			while (mEachConfigLine[lineIndex].find("location") == std::string::npos)
			{
				if (mEachConfigLine[lineIndex].find(web::serverDirective[web::ServerDirective::IP]) != std::string::npos)
					ip = web::split(mEachConfigLine[lineIndex], " ").back();
				if (mEachConfigLine[lineIndex].find(web::serverDirective[web::ServerDirective::PORT]) != std::string::npos)
					port = web::split(mEachConfigLine[lineIndex], " ").back();
				if (mEachConfigLine[lineIndex].find(web::serverDirective[web::ServerDirective::SERVER_NAME]) != std::string::npos)
					serverName = web::split(mEachConfigLine[lineIndex], " ").back();
				lineIndex++;
			}
			serverInfoLine = ip + port + serverName;
		}

		for (size_t idx = 0; idx < serverInfoList.size(); idx++)
		{
			if (serverInfoList[idx] == serverInfoLine)
				return (false);
			else
				serverInfoList.push_back(serverInfoLine);
		}
		lineIndex++;
	}
}

bool ConfigValidator::isLocationURIAlreadyExisted()
{
	size_t lineIndex = 0;
	std::string URI;
	std::vector<std::string> URIvector;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("location") != std::string::npos)
		{
			URI = web::split(mEachConfigLine[lineIndex], " {").back();
			for (size_t idx = 0; idx < URIvector.size(); idx++)
			{
				if (URIvector[idx] == URI)
					return (false);
			}
			URIvector.push_back(URI);
		}
		lineIndex++;
	}
	return (true);
}

/* inner function of isConfigSequenceMatched() */
bool ConfigValidator::isValidateServerDirective(size_t & lineIndex)
{
	size_t countFoundDirective = 0;

	/* loop until finding location block or server end */
	while (lineIndex < mEachConfigLine.size() &&
			mEachConfigLine[lineIndex].find("location") == std::string::npos &&
			mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		/* set map value zero to initialize */
		initializeCountServerDirective();
		for (size_t idx = 0; idx < NUM_SERVER_DIRECTIVE; idx++)
		{
			if (mEachConfigLine[lineIndex].find(web::serverDirective[idx]) != std::string::npos)
			{
				countFoundDirective++;
				mCountServerDirective[web::serverDirective[idx]]++;
			}
		}

		/* not matched any directive? (to find weird directive) */
		if (countFoundDirective != NUM_SERVER_DIRECTIVE)
			return (false);
		
		lineIndex++;
	}
	return (true);
}

bool ConfigValidator::isValidateLocationDirective(size_t & lineIndex)
{
	size_t countFoundDirective = 0;

	while (lineIndex < mEachConfigLine.size() &&
		mEachConfigLine[lineIndex].find("}") != std::string::npos)
	{
		/* set map value zero to initialize */
		initializeCountLocationDirective();
		for (size_t idx = 0; idx < NUM_LOCATION_DIRECTIVE; idx++)
		{
			if (mEachConfigLine[lineIndex].find(web::locationDirective[idx]) != std::string::npos)
			{
				countFoundDirective++;
				mCountLocationDirective[web::locationDirective[idx]]++;
			}
		}

		/* not matched any directive? (to find weird directive) */
		if (countFoundDirective != NUM_LOCATION_DIRECTIVE)
			return (false);

		lineIndex++;
	}
	return (true);
}

bool ConfigValidator::hasMandatoryDirective(size_t flag)
{
	if (flag == FLAG_SERVER)
	{
		/* is mandatory directive not existed? */
		if (!mCountServerDirective[web::serverDirective[web::ServerDirective::INDEX]] ||
			!mCountServerDirective[web::serverDirective[web::ServerDirective::DEFAULT_ERROR_PAGE]])
			return (false);
	}
	if (flag == FLAG_LOCATION)
	{
		/* is mandatory directive not existed? */
		if (!mCountLocationDirective[web::locationDirective[web::LocationDirective::INDEX]])
			return (false);

		/* is CGI path & extension are existed or non-existed */
		if (!mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_EXTENSION]] && mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]]
			|| mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_EXTENSION]] && !mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]])
			return (false);
	}
	return (true);
}

bool ConfigValidator::hasEachDirectiveOnlyOne(size_t flag)
{
	/* is server directive already existed? */
	if (flag == FLAG_SERVER)
	{
		for (int idx = 0; idx < NUM_SERVER_DIRECTIVE; idx++)
		{
			if (mCountServerDirective[web::serverDirective[idx]] > 1)
				return (false);
		}
	}

	/* is location directive already existed? */
	if (flag == FLAG_LOCATION)
	{
		for (size_t idx = 0; idx < NUM_LOCATION_DIRECTIVE; idx++)
		{
			if (mCountLocationDirective[web::locationDirective[idx]] > 1)
				return (false);
		}
	}
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

void ConfigValidator::readConfigFileByLine()
{
	std::ifstream configFile;
	std::string line;

	configFile.open(mFilePath);
	if (!configFile.is_open())
		throw ConfigValidator::ConfigValidatorException("The config file can't opened.");

	while (getline(configFile, line))
	{
		mEachConfigLine.push_back(line);
	}
	configFile.close();
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

