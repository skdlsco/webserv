#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
# include "ServerConfig.hpp"
# include "LocationConfig.hpp"
# include "CommonDirectives.hpp"

class ConfigParser
{
	private:
		std::vector<ServerConfig> mServers;
		std::vector<LocationConfig> mLocations;
		CommonDirectives			mCommonDirectives;
		ConfigParser();

	public:
		static std::string const TAG;
		ConfigParser(std::string configPath);
		ConfigParser(ConfigParser const & copy);
		ConfigParser &operator=(ConfigParser const & rhs);
		virtual ~ConfigParser();

		void readConfigLineByLine(std::string configPath);
		

		void addServer(ServerConfig Server);
		void addLocation(LocationConfig Location);

		void clearLocation();
		
		/* CommonDirectives */
		void parseIndexFileList();
		void parseRoot();
		void parseAutoIndex();

		/* ServerConfig */
		void parseListen();
		void parseServerName();
		void parseDefaultErrorPage();
		void parseClientMaxBodySize();

		/* LocationConfig*/
		void parseAllowMethodList();
		void parseCGIExtensionList();
		void parseCGIPath();
};

#endif