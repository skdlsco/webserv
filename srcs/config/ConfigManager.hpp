#ifndef CONFIG_MANAGER_HPP
# define CONFIG_MANAGER_HPP

#include "ConfigParser.hpp"
#include "ConfigValidator.hpp"

class ConfigManager
{
    private:
        std::string mFilePath;
        ConfigValidator validator;
        ConfigParser parser;

        ConfigManager();

    public:
        static std::string const TAG;
        ConfigManager(ConfigManager const & copy);
        ConfigManager(std::string filePath);
        ConfigManager &operator=(ConfigManager const & rhs);
        virtual ~ConfigManager();

        bool isConfigValidate();
        std::vector<ServerConfig *> parseConfigFile();
};

#endif