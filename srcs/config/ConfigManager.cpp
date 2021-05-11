#include "ConfigManager.hpp"

std::string const ConfigManager::TAG = "ConfigManager";

ConfigManager::ConfigManager(std::string filePath)
: mFilePath(filePath), mValidator(mFilePath), mParser(mFilePath)
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
        this->mValidator = rhs.mValidator;
        this->mParser = rhs.mParser;
    }
    return (*this);
}

ConfigManager::~ConfigManager()
{

}

bool ConfigManager::isConfigValidate()
{
    if (!mValidator.isConfigValidate())
        return (false);
    return (true);
}

std::vector<ServerConfig *> ConfigManager::parseConfigFile()
{
    return (mParser.parseConfigFile());
}