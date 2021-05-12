#include "ConfigParser.hpp"

std::string const ConfigParser::TAG = "ConfigParser";

ConfigParser::ConfigParser()
{

}

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

	while (lineIndex < mEachConfigLine.size())
	{
		splitResult = web::split(mEachConfigLine[lineIndex], " \t{");
		if (splitResult.front() == "server")
		{
			serverList.push_back(parseServerDirective(lineIndex));
			mCurrentServerConfig = serverList[currentServerIndex];
			currentServerIndex++;
		}
		else if (splitResult.front() == "location")
		{
			URI = addSlashToURI(splitResult.back());
			mCurrentServerConfig->addLocation(URI, parseLocationDirective(lineIndex));
		}
		else
			lineIndex++;
	}
	/* default server checker */
	setDefaultServer(serverList);
	return (serverList);
}

ServerConfig *ConfigParser::parseServerDirective(size_t & lineIndex)
{
	std::string currentLine;
	std::string currentDirective;
	std::string currentDirectiveValue;
	std::vector<std::string> splitResult;

	ServerConfig *serverConfig = new ServerConfig();
	if (!serverConfig)
		throw ConfigParserException("serverConfig cannot allocate memory.");

	while (lineIndex < mEachConfigLine.size() &&
			mEachConfigLine[lineIndex].find("location") == std::string::npos &&
			mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		currentLine = mEachConfigLine[lineIndex];
		splitResult = web::split(currentLine, " \t");

		currentDirective = splitResult.front();
		currentDirectiveValue = splitResult.back();

		/* find common directive */
		if (currentDirective == web::commonDirective[web::CommonDirective::INDEX])
			serverConfig->setIndexFile(currentDirectiveValue);
		if (currentDirective == web::commonDirective[web::CommonDirective::ROOT])
			serverConfig->setRoot(currentDirectiveValue);
		if (currentDirective == web::commonDirective[web::CommonDirective::AUTOINDEX] && currentDirectiveValue == "on")
			serverConfig->setAutoIndex(true);

		/* find server directive */
		if (currentDirective == web::serverDirective[web::ServerDirective::IP])
			serverConfig->setIP(currentDirectiveValue);
		if (currentDirective == web::serverDirective[web::ServerDirective::PORT])
			serverConfig->setPort(web::stoi(currentDirectiveValue));
		if (currentDirective == web::serverDirective[web::ServerDirective::SERVER_NAME])
			serverConfig->setServerName(currentDirectiveValue);
		if (currentDirective == web::serverDirective[web::ServerDirective::CLIENT_MAX_BODY_SIZE])
			serverConfig->setClientMaxBodySize(web::stoi(currentDirectiveValue));
		if (currentDirective == web::serverDirective[web::ServerDirective::DEFAULT_ERROR_PAGE])
			serverConfig->setDefaultErrorPagePath(currentDirectiveValue);

		lineIndex++;
	}
	return (serverConfig);
}

LocationConfig *ConfigParser::parseLocationDirective(size_t & lineIndex)
{
	std::string currentLine;
	std::string currentDirective;
	std::string currentDirectiveValue;
	std::vector<std::string> splitResult;

	LocationConfig *locationConfig = new LocationConfig();
	if (!locationConfig)
		throw ConfigParserException("locationConfig cannot allocate memory.");

	/* setting common directive use server block's value */
	setLocationConfigCommonDirective(locationConfig);
	
	while (lineIndex < mEachConfigLine.size() &&
		mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		currentLine = mEachConfigLine[lineIndex];
		splitResult = web::split(currentLine, " \t");

		currentDirective = splitResult.front();
		currentDirectiveValue = splitResult.back();

		/* find common directive */
		if (currentDirective == web::commonDirective[web::CommonDirective::INDEX])
			locationConfig->setIndexFile(currentDirectiveValue);
		if (currentDirective == web::commonDirective[web::CommonDirective::ROOT])
			locationConfig->setRoot(currentDirectiveValue);
		if (currentDirective == web::commonDirective[web::CommonDirective::AUTOINDEX] && currentDirectiveValue == "on")
			locationConfig->setAutoIndex(true);
		if (currentDirective == web::commonDirective[web::CommonDirective::AUTOINDEX] && currentDirectiveValue == "off")
			locationConfig->setAutoIndex(false);

		/* find location directive */
		if (currentDirective == web::locationDirective[web::LocationDirective::ALLOW_METHOD])
		{
			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				locationConfig->addAllowMethod(splitResult[idx]);
			}
		}
		if (currentDirective == web::locationDirective[web::LocationDirective::CGI_EXTENSION])
		{
			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				locationConfig->addCGIExtension(splitResult[idx]);
			}
		}
		if (currentDirective == web::locationDirective[web::LocationDirective::CGI_PATH])
			locationConfig->setCGIPath(splitResult.back());

		lineIndex++;
	}
	return (locationConfig);
}

void ConfigParser::setDefaultServer(std::vector<ServerConfig *> & serverList)
{
	bool isDefaultServer;
	std::string key;
	std::map<std::string, std::string> defaultServer;

	for (size_t serverIdx = 0; serverIdx < serverList.size(); serverIdx++)
	{
		isDefaultServer = true;

		/* will be make function itos() */ 
		key = serverList[serverIdx]->getIP() + web::toString(serverList[serverIdx]->getPort());
		if (defaultServer.count(key))
			isDefaultServer = false;
		serverList[serverIdx]->setDefaultServer(isDefaultServer);
	}
}

void ConfigParser::setLocationConfigCommonDirective(LocationConfig * locationConfig)
{
	locationConfig->setIndexFile(mCurrentServerConfig->getIndexFile());
	locationConfig->setRoot(mCurrentServerConfig->getRoot());
	locationConfig->setAutoIndex(mCurrentServerConfig->isAutoIndex());
}

void ConfigParser::readConfigFileByLine()
{
	std::ifstream configFile;
	std::string line;

	configFile.open(mFilePath.c_str());
	if (!configFile.is_open())
		throw ConfigParser::ConfigParserException("The config file can't opened.");

	while (getline(configFile, line))
	{
		if (line != "")
			mEachConfigLine.push_back(line);
	}
	configFile.close();
}

std::string const ConfigParser::addSlashToURI(std::string const & URI)
{
	std::string newURI;

	newURI = URI;
	if (URI == "/")
		return (newURI);

	/* will modify string.hpp (addPrefix/SuffixToString function) */
	if (URI[0] != '/')
		newURI = "/"  + URI;
	if (URI[URI.length() - 1] != '/')
		newURI = URI + "/";

	return (newURI);
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

