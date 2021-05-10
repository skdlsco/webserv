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
		this->mServerList = rhs.mServerList;
		this->mLocationList = rhs.mLocationList;
		this->mCommonDirective = rhs.mCommonDirective;
	}
	return (*this);
}

ConfigParser::~ConfigParser()
{
	mServerList.clear();
	mLocationList.clear();
}

void ConfigParser::parseConfigFile()
{
	size_t lineIndex = 0;
	std::vector <std::string> splitResult;
	std::string URI;

	while (lineIndex < mEachConfigLine.size())
	{
		splitResult = web::split(mEachConfigLine[lineIndex], " {");
		if (splitResult.front() == "server")
		{
  			parseServerDirective(lineIndex);
		}
		if (splitResult.front() == "location")
		{
			URI = splitResult.back();
			parseLocationDirective(lineIndex, URI);
		}
		lineIndex++;
	}
}

void ConfigParser::parseServerDirective(size_t & lineIndex)
{
	ServerConfig serverConfig;
	std::vector<std::string> splitResult;
	std::string currLine;

	while (lineIndex < mEachConfigLine.size() &&
			mEachConfigLine[lineIndex].find("location") == std::string::npos &&
			mEachConfigLine[lineIndex].find("}") == std::string::npos)
	{
		currLine = mEachConfigLine[lineIndex];
		splitResult = web::split(currLine, " ");

		if (currLine.find(web::serverDirective[web::ServerDirective::INDEX]) != std::string::npos)
			serverConfig.getCommonDirective().setIndexFile(splitResult.back());
		if (currLine.find(web::serverDirective[web::ServerDirective::ROOT]) != std::string::npos)
			serverConfig.getCommonDirective().setRoot(splitResult.back());
		if (currLine.find(web::serverDirective[web::ServerDirective::AUTOINDEX]) != std::string::npos && splitResult.back() == "on")
			serverConfig.getCommonDirective().setAutoIndex(true);

		if (currLine.find(web::serverDirective[web::ServerDirective::IP]) != std::string::npos)
			serverConfig.setIP(splitResult.back());
		if (currLine.find(web::serverDirective[web::ServerDirective::PORT]) != std::string::npos)
			serverConfig.setPort(atoi(splitResult.back()));
		if (currLine.find(web::serverDirective[web::ServerDirective::SERVER_NAME]) != std::string::npos)
			serverConfig.setServerName(splitResult.back());
		if (currLine.find(web::serverDirective[web::ServerDirective::CLIENT_MAX_BODY_SIZE]) != std::string::npos)
			serverConfig.setClientMaxBodySize(atoi(splitResult.back()));
		if (currLine.find(web::serverDirective[web::ServerDirective::DEFAULT_ERROR_PAGE]) != std::string::npos)
			serverConfig.setDefaultErrorPagePath(splitResult.back());

		lineIndex++;
	}

}

void ConfigParser::parseLocationDirective(size_t & lineIndex, std::string const & URI)
{
	LocationConfig locationConfig;
	std::vector<std::string> splitResult;
	std::string currLine;

	while (lineIndex < mEachConfigLine.size() &&
		mEachConfigLine[lineIndex].find("}") != std::string::npos)
	{
		currLine = mEachConfigLine[lineIndex];
		splitResult = web::split(currLine, " ");

		if (currLine.find(web::locationDirective[web::LocationDirective::INDEX]) != std::string::npos)
			locationConfig.getCommonDirective().setIndexFile(splitResult.back());
		if (currLine.find(web::locationDirective[web::LocationDirective::ROOT]) != std::string::npos)
			locationConfig.getCommonDirective().setRoot(splitResult.back());
		if (currLine.find(web::locationDirective[web::LocationDirective::AUTOINDEX]) != std::string::npos && splitResult.back() == "on")
			locationConfig.getCommonDirective().setAutoIndex(true);

		if (currLine.find(web::locationDirective[web::LocationDirective::ALLOW_METHOD]) != std::string::npos)
		{
			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				locationConfig.addAllowMethod(splitResult[idx]);
			}
		}
		if (currLine.find(web::locationDirective[web::LocationDirective::CGI_EXTENSION]) != std::string::npos)
		{
			for (size_t idx = 1; idx < splitResult.size(); idx++)
			{
				locationConfig.addCGIExtension(splitResult[idx]);
			}
		}
		if (currLine.find(web::locationDirective[web::LocationDirective::CGI_PATH]) != std::string::npos)
			locationConfig.setCGIPath(splitResult.back());
		lineIndex++;
	}
	addLocation({URI, locationConfig});
}

size_t ConfigParser::getServerBlockNum()
{
	return (mServerList.size());
}

size_t ConfigParser::getLocationBlockNum(ServerConfig const & server)
{
	return (server.getLocationList().size());
}

void ConfigParser::addServer(ServerConfig const & server)
{
	mServerList.push_back(server);
}

void ConfigParser::addLocation(std::string URI, LocationConfig const & location)
{
	mLocationList.insert({URI, location});
}

void ConfigParser::clearServerList()
{
	mServerList.clear();
}

void ConfigParser::clearLocationList()
{
	mLocationList.clear();
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

