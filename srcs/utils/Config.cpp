#include "Config.hpp"

ServerConfig *web::getConfigMatchedWithHost(std::string const & host, const std::vector<ServerConfig *> & configList)
{
	for (std::vector<ServerConfig *>::const_iterator iter = configList.begin(); iter != configList.end(); iter++)
	{
		if ((*iter)->getServerName() == host)
			return (*iter);
	}
	return (web::getDefaultServerConfig(configList));
}

ServerConfig *web::getDefaultServerConfig(const std::vector<ServerConfig *> & configList)
{
	for (std::vector<ServerConfig *>::const_iterator iter = configList.begin(); iter != configList.end(); iter++)
	{
		if ((*iter)->isDefaultServer())
			return (*iter);
	}

	/* not case */
	return (NULL);
}
