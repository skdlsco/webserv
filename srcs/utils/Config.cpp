#include "Config.hpp"

ServerConfig *web::getConfigMatchedWithHost(std::string const & host, std::vector<ServerConfig *> & configList)
{
	ServerConfig *defaultServer = web::getDefaultServerConfig(configList);

	for (std::vector<ServerConfig *>::iterator iter = configList.begin(); iter != configList.end(); iter++)
	{
		if ((*iter)->getServerName() == host)
			return (*iter);
	}
	return (defaultServer);
}

ServerConfig *web::getDefaultServerConfig(std::vector<ServerConfig *> & configList)
{
	for (std::vector<ServerConfig *>::iterator iter = configList.begin(); iter != configList.end(); iter++)
	{
		if ((*iter)->isDefaultServer())
			return (*iter);
	}

	/* not case */
	return (NULL);
}