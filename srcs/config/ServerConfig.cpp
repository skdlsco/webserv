#include "ServerConfig.hpp"

std::string const ServerConfig::TAG = "ServerConfig";

ServerConfig::ServerConfig()
: mCommonDirective(), mIP("0.0.0.0"), mPort(8080),
	mServerName(mIP), mDefaultErrorPagePath(""), mLocationList()
{

}

ServerConfig::~ServerConfig()
{
	for (LocationIter iter = mLocationList.begin(); iter != mLocationList.end(); iter++)
	{
		delete (iter->second);
	}
	mLocationList.clear();
}

ServerConfig::ServerConfig(ServerConfig const & copy)
{
	*this = copy;
}

ServerConfig &ServerConfig::operator=(ServerConfig const & rhs)
{
	if (this != &rhs)
	{
		this->mIP = rhs.mIP;
		this->mPort = rhs.mPort;
		this->mServerName = rhs.mServerName;
		this->mIsDefaultServer = rhs.mIsDefaultServer;
		this->mCommonDirective = rhs.mCommonDirective;
		this->mDefaultErrorPagePath = rhs.mDefaultErrorPagePath;

		for (LocationConstIter iter = rhs.mLocationList.begin(); iter != rhs.mLocationList.end(); iter++)
		{
			LocationConfig *locationConfig = new LocationConfig(*iter->second);

			this->mLocationList.insert(std::pair<std::string, LocationConfig *>(iter->first, locationConfig));
		}
	}
	return (*this);
}

std::string const &ServerConfig::getIndexFile() const
{
	return (mCommonDirective.getIndexFile());
}

std::string const &ServerConfig::getRoot() const
{
	return (mCommonDirective.getRoot());
}

size_t ServerConfig::getClientMaxBodySize() const
{
	return (mCommonDirective.getClientMaxBodySize());
}

bool ServerConfig::isAutoIndex() const
{
	return (mCommonDirective.isAutoIndex());
}

std::string const &ServerConfig::getIP() const
{
	return (mIP);
}

size_t ServerConfig::getPort() const
{
	return (mPort);
}

std::string const &ServerConfig::getServerName() const
{
	return (mServerName);
}

bool ServerConfig::isDefaultServer() const
{
	return (mIsDefaultServer);
}

std::string const &ServerConfig::getDefaultErrorPagePath() const
{
	return (mDefaultErrorPagePath);
}

std::map<std::string, LocationConfig *> ServerConfig::getLocationList() const
{
	return (mLocationList);
}

void ServerConfig::setIndexFile(std::string const & indexFile)
{
	mCommonDirective.setIndexFile(indexFile);
}

void ServerConfig::setRoot(std::string const & root)
{
	mCommonDirective.setRoot(root);
}

void ServerConfig::setClientMaxBodySize(size_t clientMaxBodySize)
{
	mCommonDirective.setClientMaxBodySize(clientMaxBodySize);
}

void ServerConfig::setAutoIndex(bool autoIndex)
{
	mCommonDirective.setAutoIndex(autoIndex);
}

void ServerConfig::setIP(std::string const & ip)
{
	mIP = ip;
}

void ServerConfig::setPort(size_t port)
{
	mPort = port;
}

void ServerConfig::setServerName(std::string const & serverName)
{
	mServerName = serverName;
}

void ServerConfig::setDefaultServer(bool defaultServer)
{
	mIsDefaultServer = defaultServer;
}

void ServerConfig::setDefaultErrorPagePath(std::string const & defaultErrorPagePath)
{
	mDefaultErrorPagePath = defaultErrorPagePath;
}

void ServerConfig::addLocation(std::string URI, LocationConfig *location)
{
	mLocationList.insert(std::pair<std::string, LocationConfig *>(URI, location));
}

