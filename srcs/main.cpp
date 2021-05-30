#include "ServerManager.hpp"
#include "server/Server.hpp"
#include "config/ConfigManager.hpp"
#include "logger/Logger.hpp"

int main(void)
{
	std::string filePath;
	std::vector<std::vector<ServerConfig *> >configList;
	ServerManager serverManager;

	try
	{
		ConfigManager configManager("configs/test.conf");
		if (configManager.isConfigValidate())
		{
			configList = configManager.parseConfigFile();
			std::cout << "configList size : " << configList.size() << std::endl;
		}

		logger::println("MAIN", "server   size : " + web::toString(configList[0][0]->getClientMaxBodySize()));
		logger::println("MAIN", "location size : " + web::toString(configList[0][0]->getLocationList()["/"]->getClientMaxBodySize()));
		logger::println("MAIN", "URI : " + configList[0][0]->getLocationList()["/"]->getURI());
		for (size_t idx = 0; idx < configList.size(); idx++)
		{
			Server::create(serverManager, configList[idx]);
		}
		serverManager.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
