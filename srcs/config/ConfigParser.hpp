#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
# include "../utils/Split.hpp"
# include "../utils/Directive.hpp"
# include "ServerConfig.hpp"
# include "LocationConfig.hpp"
# include "CommonDirective.hpp"

class ConfigParser
{
	private:
		std::string mFilePath;
		std::vector<std::string> mEachConfigLine;
		std::vector<ServerConfig> mServerList;
		std::map<std::string, LocationConfig> mLocationList;
		CommonDirective mCommonDirective;
		ConfigParser();

	public:
		static std::string const TAG;
		ConfigParser(std::string filePath);
		ConfigParser(ConfigParser const & copy);
		ConfigParser &operator=(ConfigParser const & rhs);
		virtual ~ConfigParser();

		void readConfigFileByLine();

		size_t getServerBlockNum();
		size_t getLocationBlockNum(ServerConfig const & server);

		void addServer(ServerConfig const & server);
		void addLocation(std::string URI, LocationConfig const & location);

		void parseConfigFile();
		void parseServerDirective(size_t & lineIndex);
		void parseLocationDirective(size_t & lineIndex, std::string const & URI);

		void clearServerList();
		void clearLocationList();

		class ConfigParserException : public std::exception
		{
			private:
				std::string mMessage;

			public:
				virtual ~ConfigParserException() throw();
				ConfigParserException(std::string message) throw();
				virtual const char* what() const throw();
		};
};

#endif