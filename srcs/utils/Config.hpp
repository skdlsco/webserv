#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include "config/ServerConfig.hpp"
#include "config/LocationConfig.hpp"

namespace web
{
	ServerConfig *getConfigMatchedWithHost(std::string const & host, std::vector<ServerConfig *> & configList);
};

#endif