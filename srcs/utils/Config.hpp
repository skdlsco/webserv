#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include "config/ServerConfig.hpp"
#include "config/LocationConfig.hpp"

namespace web
{
	ServerConfig *getConfigMatchedWithHost(std::string const & host, const std::vector<ServerConfig *> & configList);
	ServerConfig *getDefaultServerConfig(const std::vector<ServerConfig *> & configList);
};

#endif
