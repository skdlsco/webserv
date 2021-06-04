#include "ServerManager.hpp"
#include "server/Server.hpp"
#include "config/ConfigManager.hpp"
#include "logger/Logger.hpp"

// void sigintHandler(int sig)
// {
// 	sig = 0;

// 	std::cerr << "test" << std::endl;
// 	exit(0);
// }

int main(int argc, char *argv[])
{
	std::string filePath;
	std::vector<std::vector<ServerConfig *> >configList;
	ServerManager serverManager;

	try
	{
		// signal(SIGINT, sigintHandler);
		if (argc > 2)
		{
			std::cerr << "check your input." << std::endl;
			return (-1);
		}
		else if (argc < 2)
			filePath = "./configs/default_goinfre.conf";
		else
			filePath = argv[1];
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
