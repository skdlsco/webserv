#include "ServerComponent.hpp"

const std::string ServerComponent::TAG = "ServerComponent";

ServerComponent::ServerComponent(ServerManager &serverManager) : mServerManager(serverManager)
{

}

ServerComponent::~ServerComponent()
{

}

void ServerComponent::finish()
{
	delete this;
}


ServerManager &ServerComponent::getServerManager()
{
	return (mServerManager);
}

