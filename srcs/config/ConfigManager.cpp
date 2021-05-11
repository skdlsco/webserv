#include "ConfigManager.hpp"

std::string const ConfigManager::TAG = "ConfigManager";

ConfigManager::ConfigManager(std::string filePath)
: mFilePath(filePath), validator(mFilePath), parser(mFilePath)
{

}

ConfigManager::ConfigManager(ConfigManager const & copy)
{
    *this = copy;
}

ConfigManager &ConfigManager::operator=(ConfigManager const & rhs)
{
    if (this != &rhs)
    {
        this->mFilePath = rhs.mFilePath;
        this->validator = rhs.validator;
        this->parser = rhs.parser;
    }
    return (*this);
}

ConfigManager::~ConfigManager()
{

}

bool ConfigManager::isConfigValidate()
{
    if (!validator.isConfigValidate())
        return (false);
    return (true);
}

std::vector<ServerConfig *> ConfigManager::parseConfigFile()
{
    return (parser.parseConfigFile());
}