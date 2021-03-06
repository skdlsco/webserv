#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "LocationConfig.hpp"
#include "CommonDirective.hpp"

class ServerConfig
{
	typedef std::map<std::string, LocationConfig *>::iterator LocationIter;
	typedef std::map<std::string, LocationConfig *>::const_iterator LocationConstIter;

	private:
		CommonDirective mCommonDirective;
		std::string mIP;
		size_t mPort;
		std::string	mServerName;
		bool mIsDefaultServer;
		std::string mDefaultErrorPagePath;
		std::map<std::string, LocationConfig *>	mLocationList;

	public:
		static std::string const TAG;
		ServerConfig();
		ServerConfig(ServerConfig const & copy);
		ServerConfig &operator=(ServerConfig const & rhs);
		virtual ~ServerConfig();

		std::string const &getIndexFile() const;
		std::string const &getRoot() const;
		size_t getClientMaxBodySize() const;
		bool isAutoIndex() const;

		std::string const &getIP() const;
		size_t getPort() const;
		std::string const &getServerName() const;
		bool isDefaultServer() const;
		std::string const &getDefaultErrorPagePath() const;
		std::map<std::string, LocationConfig *> getLocationList() const;

		void setIndexFile(std::string const & indexFile);
		void setRoot(std::string const & root);
		void setClientMaxBodySize(size_t clientMaxBodySize);
		void setAutoIndex(bool autoIndex);

		void setIP(std::string const & ip);
		void setPort(size_t port);
		void setServerName(std::string const & serverName);
		void setDefaultServer(bool defaultServer);
		void setDefaultErrorPagePath(std::string const & defaultErrorPagePath);

		void addLocation(std::string URI, LocationConfig *location);
};

#endif
