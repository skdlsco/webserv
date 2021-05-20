#include "Config.hpp"

ServerConfig *web::getConfigMatchedWithHost(std::string const & host, std::vector<ServerConfig *> & configList)
{
	ServerConfig *defaultServer = NULL;

	for (std::vector<ServerConfig *>::iterator iter = configList.begin(); iter != configList.end(); iter++)
	{
		if ((*iter)->getServerName() == host)
			return (*iter);
		if ((*iter)->isDefaultServer())
			defaultServer = (*iter);
	}
	return (defaultServer);
}