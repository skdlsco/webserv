#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "ConfigParser.hpp"
#include "ConfigValidator.hpp"

class Config
{
    private:
        std::string mFilePath;
        ConfigValidator validator;
        ConfigParser parser;

        Config();

    public:
        static std::string const TAG;
        Config(Config const & copy);
        Config(std::string filePath);
        Config &operator=(Config const & rhs);
        virtual ~Config();

        bool isConfigValidate();
        std::vector<ServerConfig *> parseConfigFile();
};

#endif