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
		ConfigManager configManager("configs/validate.conf");
		if (configManager.isConfigValidate())
		{
			configList = configManager.parseConfigFile();
			std::cout << "configList size : " << configList.size() << std::endl;

			for (size_t i = 0; i < configList.size(); i++)
			{
				std::cout << configList[i].size() << std::endl;
			}

			logger::print("MAIN") << std::endl;
			logger::print("MAIN") << "configList[0][0].index : " << configList[0][0]->isAutoIndex() << std::endl;
			logger::print("MAIN") << "configList[0][1].index : " << (configList[0][1]->getLocationList())["/test/"]->isAutoIndex() << std::endl;
		}

		// for (size_t idx = 0; idx < configList.size(); idx++)
		// {
		// 	Server::create(serverManager, configList[idx]);
		// }
		// serverManager.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
