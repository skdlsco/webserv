#include "Config.hpp"

Config::Config(int port)
: mPort(port)
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
		return (*this);
	this->mPort = copy.mPort;
	return (*this);
}

int Config::getPort() const
{
	return (mPort);
}
