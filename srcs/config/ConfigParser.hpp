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

		std::vector<ServerConfig *> parseConfigFile();
		ServerConfig *parseServerDirective(size_t & lineIndex);
		LocationConfig *parseLocationDirective(size_t & lineIndex);

		void setLocationConfigCommonDirective(LocationConfig * locationConfig);
		void readConfigFileByLine();

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