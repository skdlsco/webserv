#include "ServerConfig.hpp"

std::string const ServerConfig::TAG = "ServerConfig";

ServerConfig::ServerConfig() 
: mCommonDirectives(), mIP("0.0.0.0"), mPort(8080), 
	mServerName(mIP), mClientMaxBodySize(0), mDefaultErrorPagePath(""), 
	mLocationList()
{

}

ServerConfig::~ServerConfig()
{

}

ServerConfig::ServerConfig(ServerConfig const & copy)
{
	this->mIP = copy.mIP;
	this->mPort = copy.mPort;
	this->mServerName = copy.mServerName;
	this->mDefaultServer = copy.mDefaultServer;
	this->mCommonDirectives = copy.mCommonDirectives;
	this->mClientMaxBodySize = copy.mClientMaxBodySize;
	this->mDefaultErrorPagePath = copy.mDefaultErrorPagePath;
	this->mLocationList = copy.mLocationList;
}

ServerConfig &ServerConfig::operator=(ServerConfig const & rhs)
{
	if (this != &rhs)
	{
		ServerConfig(rhs);
	}
	return (*this);
}

std::string ServerConfig::getIP() const
{
	return (mIP);
}

size_t ServerConfig::getPort() const
{
	return (mPort);
}

std::string ServerConfig::getServerName() const
{
	return (mServerName);
}

bool ServerConfig::isDefaultServer() const
{
	return (mDefaultServer);
}

size_t ServerConfig::getClientMaxBodySize() const
{
	return (mClientMaxBodySize);
}

std::string ServerConfig::getDefaultErrorPagePath() const
{
	return (mDefaultErrorPagePath);
}

std::vector<LocationConfig> ServerConfig::getLocationList() const
{
	return (mLocationList);
}

void ServerConfig::setDefaultServer(bool defaultServer)
{
	mDefaultServer = defaultServer;
}

void ServerConfig::setIP(std::string ip)
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
	mDefaultServer = defaultServer;
}

void ServerConfig::setClientMaxBodySize(size_t clientMaxBodySize)
{
	mClientMaxBodySize = clientMaxBodySize;
}

void ServerConfig::setDefaultErrorPagePath(std::string const & defaultErrorPagePath)
{
	mDefaultErrorPagePath = defaultErrorPagePath;
}

void ServerConfig::addLocation(LocationConfig const & location)
{
	mLocationList.push_back(location);
}

