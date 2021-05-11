#include "Config.hpp"

std::string const Config::TAG = "Config";

Config::Config(std::string filePath)
: mFilePath(filePath), validator(mFilePath), parser(mFilePath)
{

}

Config::Config(Config const & copy)
{
    *this = copy;
}

Config &Config::operator=(Config const & rhs)
{
    if (this != &rhs)
    {
        this->mFilePath = rhs.mFilePath;
        this->validator = rhs.validator;
        this->parser = rhs.parser;
    }
    return (*this);
}

Config::~Config()
{

}

bool Config::isConfigValidate()
{
    if (!validator.isConfigValidate())
        return (false);
    return (true);
}

std::vector<ServerConfig *> Config::parseConfigFile()
{
    return (parser.parseConfigFile());
}