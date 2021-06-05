#include "ServerManager.hpp"
#include "server/Server.hpp"
#include "config/ConfigManager.hpp"
#include "logger/Logger.hpp"

bool checkConfigPath(int argc, char *argv[], std::string &filePath)
{
	if (argc > 2)
		return (false);
	else if (argc < 2)
		filePath = "./configs/default_goinfre_chlee.conf";
	else
		filePath = argv[1];
	return (true);
}

int main(int argc, char *argv[])
{
	std::string filePath;
	std::vector<std::vector<ServerConfig *> >configList;
	ServerManager serverManager;

	try
	{
		if (!checkConfigPath(argc, argv, filePath))
		{
			std::cerr << "check your input." << std::endl;
			return (1);
		}
		ConfigManager configManager(filePath);
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
