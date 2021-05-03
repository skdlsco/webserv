#include "Config.hpp"

const std::string Config::TAG = "Config";

Config::Config(int port, std::string const & host, bool defaultServer) 
: mPort(port), mHost(host), mDefaultServer(defaultServer), mCommonDirectives()
{

}

Config::~Config()
{

}

Config::Config(Config const & copy)
{
	*this = copy;
}

Config &Config::operator=(Config const & copy)
{
	if (this == &copy)
	{
		this->mPort = copy.mPort;
	}
	return (*this);
}

bool Config::isDefaultServer() const
{
	return (mDefaultServer);
}

int Config::getPort() const
{
	return (mPort);
}

std::string Config::getHost() const
{
	return (mHost);
}

void Config::setDefaultServer(bool defaultServer)
{
	mDefaultServer = defaultServer;
}

void Config::setPort(int port)
{
	mPort = port;
}

void Config::setHost(std::string const & host)
{
	mHost = host;
}
