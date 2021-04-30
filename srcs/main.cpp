#include "ServerManager.hpp"
#include "server/Server.hpp"
#include "config/Config.hpp"

int main(void)
{
	ServerManager serverManager;

	Server::create(serverManager, Config(3000));
	Server::create(serverManager, Config(8000));

	serverManager.run();
}
