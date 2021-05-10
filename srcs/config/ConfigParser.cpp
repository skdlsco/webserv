#include "ConfigParser.hpp"

std::string const ConfigParser::TAG = "ConfigParser";

ConfigParser::ConfigParser(std::string filePath)
: mFilePath(filePath)
{
	readConfigFileByLine();
}

ConfigParser::ConfigParser(ConfigParser const & copy)
{
	*this = copy;
}

ConfigParser &ConfigParser::operator=(ConfigParser const & rhs)
{
	if (this != &rhs)
	{
		this->mFilePath = rhs.mFilePath;
		this->mEachConfigLine = rhs.mEachConfigLine;
	}
	return (*this);
}

ConfigParser::~ConfigParser()
{

}

std::vector<ServerConfig *> ConfigParser::parseConfigFile()
{
	size_t lineIndex = 0;
	size_t currentServerIndex = 0;

	std::string URI;
	std::vector<std::string> splitResult;
	std::vector<ServerConfig *> serverList;
	std::map<std::string, LocationConfig *> locationList;

	while (lineIndex < mEachConfigLine.size())
	{
		splitResult = web::split(mEachConfigLine[lineIndex], " {");
		if (splitResult.front() == "server")
		{
			locationList.clear();
			serverList.push_back(parseServerDirective(lineIndex));
			mCurrentServerConfig = serverList[currentServerIndex];
			currentServerIndex++;
		}
		if (splitResult.front() == "location")
		{
			URI = splitResult.back();
			mCurrentServerConfig->addLocation(URI, parseLocationDirective(lineIndex));
		}
		lineIndex++;
	}
	return (serverList);
}

ServerConfig *ConfigParser::parseServerDirective(size_t & lineIndex)
{
	std::string currentLine;
	std::vector<std::string> splitResult;

	ServerConfig *serverConfig = new ServerConfig();
	if (!serverConfig)
		throw ConfigParserException("serverConfig cannot allocate memory.");

	while (lineIndex < mEachConfigLine.size() &&
			mEachConfigLine[lineIndex].find("location") == std::string::npos &&
			mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		currentLine = mEachConfigLine[lineIndex];
		splitResult = web::split(currentLine, " ");

		/* find common directive */
		if (currentLine.find(web::serverDirective[web::ServerDirective::INDEX]) != std::string::npos)
			serverConfig->getCommonDirective().setIndexFile(splitResult.back());
		if (currentLine.find(web::serverDirective[web::ServerDirective::ROOT]) != std::string::npos)
			serverConfig->getCommonDirective().setRoot(splitResult.back());
		if (currentLine.find(web::serverDirective[web::ServerDirective::AUTOINDEX]) != std::string::npos && splitResult.back() == "on")
			serverConfig->getCommonDirective().setAutoIndex(true);

		/* find server directive */
		if (currentLine.find(web::serverDirective[web::ServerDirective::IP]) != std::string::npos)
			serverConfig->setIP(splitResult.back());
		if (currentLine.find(web::serverDirective[web::ServerDirective::PORT]) != std::string::npos)
			serverConfig->setPort(atoi(splitResult.back().c_str()));
		if (currentLine.find(web::serverDirective[web::ServerDirective::SERVER_NAME]) != std::string::npos)
			serverConfig->setServerName(splitResult.back());
		if (currentLine.find(web::serverDirective[web::ServerDirective::CLIENT_MAX_BODY_SIZE]) != std::string::npos)
			serverConfig->setClientMaxBodySize(atoi(splitResult.back().c_str()));
		if (currentLine.find(web::serverDirective[web::ServerDirective::DEFAULT_ERROR_PAGE]) != std::string::npos)
			serverConfig->setDefaultErrorPagePath(splitResult.back());

		lineIndex++;
	}
	return (serverConfig);
}

LocationConfig *ConfigParser::parseLocationDirective(size_t & lineIndex)
{
	std::string currentLine;
	std::vector<std::string> splitResult;

	LocationConfig *locationConfig = new LocationConfig();
	if (!locationConfig)
		throw ConfigParserException("locationConfig cannot allocate memory.");

	/* setting common directive use server block's value */
	setLocationConfigCommonDirective(locationConfig);
	
	while (lineIndex < mEachConfigLine.size() &&
		mEachConfigLine[lineIndex].find("}") != std::string::npos)
	{
		currentLine = mEachConfigLine[lineIndex];
		splitResult = web::split(currentLine, " ");

		/* find common directive */
		if (currentLine.find(web::locationDirective[web::LocationDirective::INDEX]) != std::string::npos)
			locationConfig->getCommonDirective().setIndexFile(splitResult.back());
		if (currentLine.find(web::locationDirective[web::LocationDirective::ROOT]) != std::string::npos)
			locationConfig->getCommonDirective().setRoot(splitResult.back());
		if (currentLine.find(web::locationDirective[web::LocationDirective::AUTOINDEX]) != std::string::npos && splitResult.back() == "on")
			locationConfig->getCommonDirective().setAutoIndex(true);

		/* find location directive */
		if (currentLine.find(web::locationDirective[web::LocationDirective::ALLOW_METHOD]) != std::string::npos)
		{
			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				locationConfig->addAllowMethod(splitResult[idx]);
			}
		}
		if (currentLine.find(web::locationDirective[web::LocationDirective::CGI_EXTENSION]) != std::string::npos)
		{
			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				locationConfig->addCGIExtension(splitResult[idx]);
			}
		}
		if (currentLine.find(web::locationDirective[web::LocationDirective::CGI_PATH]) != std::string::npos)
			locationConfig->setCGIPath(splitResult.back());

		lineIndex++;
	}
	return (locationConfig);
}

void ConfigParser::setLocationConfigCommonDirective(LocationConfig * locationConfig)
{
	locationConfig->getCommonDirective().setIndexFile(mCurrentServerConfig->getCommonDirective().getIndexFile());
	locationConfig->getCommonDirective().setRoot(mCurrentServerConfig->getCommonDirective().getRoot());
	locationConfig->getCommonDirective().setAutoIndex(mCurrentServerConfig->getCommonDirective().isAutoIndex());
}

void ConfigParser::readConfigFileByLine()
{
	std::ifstream configFile;
	std::string line;

	configFile.open(mFilePath);
	if (!configFile.is_open())
		throw ConfigParser::ConfigParserException("The config file can't opened.");

	while (getline(configFile, line))
	{
		mEachConfigLine.push_back(line);
	}
	configFile.close();
}

ConfigParser::ConfigParserException::ConfigParserException(std::string message) throw()
{
	mMessage = "CONFIG PARSER ERROR: " + message;
}

ConfigParser::ConfigParserException::~ConfigParserException() throw()
{

}

const char* ConfigParser::ConfigParserException::what() const throw()
{
	return (mMessage.c_str());
}

