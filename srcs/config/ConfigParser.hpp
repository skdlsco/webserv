#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "file/File.hpp"
#include "utils/Directive.hpp"
#include "utils/String.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

class ConfigParser
{
	private:
		std::string mFilePath;
		std::vector<std::string> mEachConfigLine;
		ServerConfig *mCurrentServerConfig;
		ConfigParser();
	public:
		static std::string const TAG;

		ConfigParser(std::string filePath);
		ConfigParser(ConfigParser const & copy);
		ConfigParser &operator=(ConfigParser const & rhs);
		virtual ~ConfigParser();

		std::vector<std::vector<ServerConfig *> > parseConfigFile();
		ServerConfig *parseServerDirective(size_t & lineIndex);
		LocationConfig *parseLocationDirective(size_t & lineIndex);

		void setDefaultServer(std::vector<ServerConfig *> & serverList);
		void setLocationConfigCommonDirective(LocationConfig * locationConfig);
		void readConfigFileByLine();
		std::string const addSlashToURI(std::string const & URI);
		std::vector<std::vector<ServerConfig *> > classifyServerListByAddress(std::vector<ServerConfig *> & serverList);

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