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
		ConfigManager configManager("configs/test_ina.conf");
		if (configManager.isConfigValidate())
		{
			configList = configManager.parseConfigFile();
		}

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
