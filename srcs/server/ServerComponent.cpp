#include "ServerComponent.hpp"

const std::string ServerComponent::TAG = "ServerComponent";

ServerComponent::ServerComponent(ServerManager &serverManager) : mServerManager(serverManager)
{
	mServerManager.addComponent(this);
}

ServerComponent::~ServerComponent()
{
	mServerManager.removeComponent(this);
}

void ServerComponent::finish()
{
	delete this;
}

ServerManager &ServerComponent::getServerManager()
{
	return (mServerManager);
}

