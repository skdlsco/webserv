#include "ServerComponent.hpp"

std::string const ServerComponent::TAG = "ServerComponent";

ServerComponent::ServerComponent(ServerManager &serverManager)
: mServerManager(serverManager)
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

ServerManager &ServerComponent::getServerManager() const
{
	return (mServerManager);
}

