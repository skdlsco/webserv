#include "ConfigValidator.hpp"
#include "utils/String.hpp"

std::string const ConfigValidator::TAG = "ConfigValidator";
std::string const ConfigValidator::DEFAULT_IP = "0.0.0.0";
std::string const ConfigValidator::DEFAULT_PORT = "8080";
std::string const ConfigValidator::DEFAULT_SERVER_NAME = "0.0.0.0";

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
	if (isServerInfoAlreadyExisted())
		throw ConfigValidator::ConfigValidatorException("Two or more server blocks have same server_name, ports, and ip.");
	if (isLocationURIAlreadyExisted())
		throw ConfigValidator::ConfigValidatorException("The config file contains at least two identical URIs.");
	if (!isValidateMethodName())
		throw ConfigValidator::ConfigValidatorException("The config file contains weird method name.");
	if (!isValidateExtension())
		throw ConfigValidator::ConfigValidatorException("The config file contains weird extension.");
	if (!isValidateAuth())
		throw ConfigValidator::ConfigValidatorException("The config file contains weird auth.");
	return (true);
}

bool ConfigValidator::isScopeMatched()
{
	int		scopeValue = 0;
	std::string currentLine;
	std::vector<std::string> splitResult;

	for (size_t i = 0; i < mEachConfigLine.size(); i++)
	{
		if (currentLine.find('{') != std::string::npos || 
			currentLine.find('}') != std::string::npos)
		{
			splitResult = web::split(currentLine, " \t");
			if ((splitResult.size() == 2 && (splitResult.front() == "server" && splitResult.back() == "{")) ||
				(splitResult.size() == 3 && (splitResult.front() == "location" && splitResult.back() == "{")))
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
		splitResult = web::split(mEachConfigLine[lineIndex], " \t{");
		if (splitResult.front() == "server")
		{
			lineIndex++;
			if (!isValidateServerDirective(lineIndex))
				return (false);
			if (!hasMandatoryDirective(FLAG_SERVER) || !hasEachDirectiveOnlyOne(FLAG_SERVER))
				return (false);
		}
		else if (splitResult.front() == "location")
		{
			lineIndex++;
			if (!isValidateLocationDirective(lineIndex))
				return (false);
			if (!hasMandatoryDirective(FLAG_LOCATION) || !hasEachDirectiveOnlyOne(FLAG_LOCATION))
				return (false);
		}
		else if (splitResult.front() == "}")
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

	/* can't use set because subject..*/
	std::vector<std::string> serverInfoList;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("server") != std::string::npos)
		{
			lineIndex++;

			/* initalize to default value */
			ip = DEFAULT_IP;
			port = DEFAULT_PORT;
			serverName = DEFAULT_SERVER_NAME;

			while (mEachConfigLine[lineIndex].find("location") == std::string::npos)
			{
				if (mEachConfigLine[lineIndex].find(web::serverDirective[web::ServerDirective::IP]) != std::string::npos)
					ip = web::split(mEachConfigLine[lineIndex], " \t").back();
				if (mEachConfigLine[lineIndex].find(web::serverDirective[web::ServerDirective::PORT]) != std::string::npos)
					port = web::split(mEachConfigLine[lineIndex], " \t").back();
				if (mEachConfigLine[lineIndex].find(web::serverDirective[web::ServerDirective::SERVER_NAME]) != std::string::npos)
					serverName = web::split(mEachConfigLine[lineIndex], " \t").back();
				lineIndex++;
			}
			serverInfoLine = ip + port + serverName;

			/* port range check */
			if (web::stoi(port) < 0 || web::stoi(port) > 65535)
				return (true);

			if (std::find(serverInfoList.begin(), serverInfoList.end(), serverInfoLine) != serverInfoList.end())
				return (true);
			else
				serverInfoList.push_back(serverInfoLine);
		}
		else
			lineIndex++;
	}
	return (false);
}

bool ConfigValidator::isLocationURIAlreadyExisted()
{
	size_t lineIndex = 0;
	std::string URI;
	std::vector<std::string> URIvector;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("server") != std::string::npos)
		{
			/* to meet the conditions of loop */
			lineIndex++;
			URIvector.clear();

			while (lineIndex < mEachConfigLine.size() && mEachConfigLine[lineIndex].find("server") == std::string::npos)
			{
				if (mEachConfigLine[lineIndex].find("location") != std::string::npos)
				{
					URI = web::split(mEachConfigLine[lineIndex], " \t{").back();
					if (std::find(URIvector.begin(), URIvector.end(), URI) != URIvector.end())
						return (true);
					else
						URIvector.push_back(URI);
				}
				lineIndex++;
			}
		}
	}
	return (false);
}

/* inner function of isConfigSequenceMatched() */
bool ConfigValidator::isValidateServerDirective(size_t & lineIndex)
{
	bool foundDirective;

	/* set map value zero to initialize */
	initializeCountServerDirective();

	/* loop until finding location block or server end */
	while (lineIndex < mEachConfigLine.size() &&
			mEachConfigLine[lineIndex].find("location") == std::string::npos &&
			mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		foundDirective = false;
		for (size_t idx = 0; idx < NUM_SERVER_DIRECTIVE; idx++)
		{
			if (web::split(mEachConfigLine[lineIndex], " \t").front() == web::serverDirective[idx])
			{
				foundDirective = true;
				mCountServerDirective[web::serverDirective[idx]]++;
			}
		}

		/* not matched any directive? (to find weird directive) */
		if (!foundDirective)
			return (false);

		lineIndex++;
	}
	return (true);
}

bool ConfigValidator::isValidateMethodName()
{
	size_t lineIndex = 0;
	bool foundDirective = false;
	std::vector<std::string> methodVector;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("allow_method") != std::string::npos ||
			mEachConfigLine[lineIndex].find("cgi_method") != std::string::npos)
		{
			methodVector = web::split(mEachConfigLine[lineIndex], " \t");
			
			for (size_t splitIdx = 1; splitIdx < methodVector.size(); splitIdx++)
			{
				foundDirective = false;
				for (size_t methodIdx = 0; methodIdx < NUM_METHOD_LIST; methodIdx++)
				{
					if (methodVector[splitIdx] == web::method[methodIdx])
						foundDirective = true;
				}

				/* not matched any method? (to find weird method) */
				if (!foundDirective)
					return (false);
			}
		}
		lineIndex++;
	}
	return (true);
}

bool ConfigValidator::isValidateExtension()
{
	size_t lineIndex = 0;
	std::string extension;
	std::vector<std::string> extensionVector;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("cgi_extension") != std::string::npos)
		{
			extensionVector = web::split(mEachConfigLine[lineIndex], " \t");
			
			for (size_t splitIdx = 1; splitIdx < extensionVector.size(); splitIdx++)
			{
				extension = extensionVector[splitIdx].substr(extensionVector[splitIdx].find('.') + 1);
				if (extension.length() <= 0)
					return (false);
				for (size_t extensionIdx = 0; extensionIdx < extension.length(); extensionIdx++)
				{
					if (!web::isAlpha(extension[extensionIdx]))
						return (false);
				}
			}
		}
		lineIndex++;
	}
	return (true);
}

bool ConfigValidator::isValidateAuth()
{
	size_t lineIndex = 0;
	size_t colonIndex;
	std::string auth;

	while (lineIndex < mEachConfigLine.size())
	{
		if (mEachConfigLine[lineIndex].find("auth") != std::string::npos)
		{
			auth = web::split(mEachConfigLine[lineIndex], " \t").back();
			colonIndex = auth.find(":");
			if (colonIndex == std::string::npos)
				return (false);
			if (auth.substr(0, colonIndex) == "" || auth.substr(colonIndex + 1) == "")
				return (false);
		}
		lineIndex++;
	}
	return (true);
}

bool ConfigValidator::isValidateLocationDirective(size_t & lineIndex)
{
	bool foundDirective;

	/* set map value zero to initialize */
	initializeCountLocationDirective();

	while (lineIndex < mEachConfigLine.size() &&
		mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		foundDirective = false;
		for (size_t idx = 0; idx < NUM_LOCATION_DIRECTIVE; idx++)
		{
			if (web::split(mEachConfigLine[lineIndex], " \t").front() == web::locationDirective[idx])
			{
				foundDirective = true;
				mCountLocationDirective[web::locationDirective[idx]]++;
			}
		}

		/* not matched any directive? (to find weird directive) */
		if (!foundDirective)
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
		if ((!mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_EXTENSION]] && mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]])
			|| (mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_EXTENSION]] && !mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]]))
			return (false);

		/* is CGI Vector & CGI Path are existed or non-existed */
		if ((!mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]] && mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_METHOD]])
			|| (mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_PATH]] && !mCountLocationDirective[web::locationDirective[web::LocationDirective::CGI_METHOD]]))
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

	return (true);
}

void ConfigValidator::initializeCountServerDirective()
{
	mCountServerDirective.clear();
	for (size_t i = 0; i < NUM_SERVER_DIRECTIVE; i++)
	{
		mCountServerDirective.insert(std::pair<std::string, int>(web::serverDirective[i], 0));
	}
}

void ConfigValidator::initializeCountLocationDirective()
{
	mCountLocationDirective.clear();
	for (size_t i = 0; i < NUM_LOCATION_DIRECTIVE; i++)
	{
		mCountLocationDirective.insert(std::pair<std::string, int>(web::locationDirective[i], 0));
	}
}

void ConfigValidator::readConfigFileByLine()
{
	File file(mFilePath);
	size_t commentIndex;
	std::ifstream configFile;
	std::string line;

	file.openFile();
	while (!file.isStateDone())
	{
		line = file.getLine();
		web::trim(line, " \t");
		commentIndex = line.find('#');
		if (commentIndex != std::string::npos)
			line.erase(commentIndex);
		if (line != "")
			mEachConfigLine.push_back(line);
	}
	file.closeFile();
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

