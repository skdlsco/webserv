#ifndef CONFIG_MANAGER_HPP
# define CONFIG_MANAGER_HPP

#include "ConfigParser.hpp"
#include "ConfigValidator.hpp"

class ConfigManager
{
	private:
		ConfigValidator mValidator;
		ConfigParser mParser;
		ConfigManager();

	public:
		static std::string const TAG;
		ConfigManager(std::string filePath);
		ConfigManager(ConfigManager const & copy);
		ConfigManager &operator=(ConfigManager const & rhs);
		virtual ~ConfigManager();

		bool isConfigValidate();
		std::vector<std::vector<ServerConfig *> > parseConfigFile();
};

#endif