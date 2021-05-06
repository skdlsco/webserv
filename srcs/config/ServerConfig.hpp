#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
# include "LocationConfig.hpp"
# include "ConfigParser.hpp"
# include "CommonDirectives.hpp"

class ServerConfig
{
	typedef std::vector<LocationConfig>::iterator LocationIter;
	
	private:
		CommonDirectives		mCommonDirectives;

		/* below attributes have default value */
		std::string				mIP;
		size_t					mPort;
		std::string				mServerName;
		bool					mDefaultServer;
		size_t 					mClientMaxBodySize;

		/* below attributes haven't default value */
		std::string				mDefaultErrorPagePath;
		std::vector<LocationConfig>	mLocationList;
	
	public:
		static std::string const TAG;
		ServerConfig();
		ServerConfig(ServerConfig const & copy);
		ServerConfig &operator=(ServerConfig const & rhs);
		virtual ~ServerConfig();

		std::string	getIP() const;
		size_t		getPort() const;
		std::string getServerName() const;
		bool		isDefaultServer() const;
		size_t		getClientMaxBodySize() const;
		std::string	getDefaultErrorPagePath() const;
		std::vector<LocationConfig> getLocationList() const;
		// LocationConfig getLocationByIndex(size_t idx) const;
	
		void setIP(std::string ip);
		void setPort(size_t port);
		void setServerName(std::string const & serverName);
		void setDefaultServer(bool defaultServer);
		void setClientMaxBodySize(size_t clientMaxBodySize);
		void setDefaultErrorPagePath(std::string const & defaultErrorPagePath);

		void addLocation(LocationConfig const & location);
		// void removeLocation(LocationConfig & location);

};

#endif
