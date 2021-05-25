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

std::vector<std::vector<ServerConfig *> > ConfigParser::parseConfigFile()
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

	return (classifyServerListByAddress(serverList));
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
			std::vector<std::string> allowMethodList = locationConfig->getAllowMethodList();

			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				if (std::find(allowMethodList.begin(),
								allowMethodList.end(),
								splitResult[idx]) != allowMethodList.end())
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
			locationConfig->setCGIPath(currentDirectiveValue);
		if (currentDirective == web::locationDirective[web::LocationDirective::AUTH])
		{
			locationConfig->setAuthUserName(currentDirectiveValue.substr(0, currentDirectiveValue.find(":")));
			locationConfig->setAuthUserPassword(currentDirectiveValue.substr(currentDirectiveValue.find(":") + 1));
		}
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
		else
			defaultServer.insert(std::pair<std::string, std::string>(key, serverList[serverIdx]->getServerName()));
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
	File file(mFilePath);
	std::ifstream configFile;
	std::string line;

	file.openFile();
	while (!file.isStateDone())
	{
		line = file.getLine();
		web::trim(line);
		if (line[0] == '#')
			continue;
		if (line != "")
			mEachConfigLine.push_back(line);
	}
	file.closeFile();
}

std::string const ConfigParser::addSlashToURI(std::string const & URI)
{
	std::string newURI;

	newURI = URI;
	if (URI == "/")
		return (newURI);

	/* will modify string.hpp (addPrefix/SuffixToString function) */
	if (URI[0] != '/')
		web::addPrefixToString(newURI, "/");
	if (URI[URI.length() - 1] != '/')
		web::addSuffixToString(newURI, "/");

	return (newURI);
}

std::vector<std::vector<ServerConfig *> > ConfigParser::classifyServerListByAddress(std::vector<ServerConfig *> & serverList)
{
	std::vector<std::vector<ServerConfig *> > Result;
	std::vector<ServerConfig *> sameAddressConfig;
	std::vector<std::string> addressList;
	std::string currentAddress;

	/* to set key */
	for (std::vector<ServerConfig *>::iterator iter = serverList.begin(); iter != serverList.end(); iter++)
	{
		currentAddress = (*iter)->getIP() + web::toString((*iter)->getPort());
		if (std::find(addressList.begin(), addressList.end(), currentAddress) == addressList.end())
			addressList.push_back(currentAddress);
	}

	for (std::vector<std::string>::iterator addressListIter = addressList.begin(); addressListIter != addressList.end(); addressListIter++)
	{
		sameAddressConfig.clear();
		for (std::vector<ServerConfig *>::iterator serverConfigIter = serverList.begin(); serverConfigIter != serverList.end(); serverConfigIter++)
		{
			currentAddress = (*serverConfigIter)->getIP() + web::toString((*serverConfigIter)->getPort());
			if ((*addressListIter) == currentAddress)
				sameAddressConfig.push_back(*serverConfigIter);
		}
		Result.push_back(sameAddressConfig);
	}
	return (Result);
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

