#include "ServerManager.hpp"

const std::string ServerManager::TAG = "ServerManager";

ServerManager::ServerManager()
{

}

ServerManager::~ServerManager()
{
	for (componentIter it = mServerComponentVec.begin(); it < mServerComponentVec.end(); it++)
	{
		(*it)->finish();
	}
}

void ServerManager::run()
{
	while (1)
	{
		mFDManager.select();
		for (componentIter it = mServerComponentVec.begin(); it < mServerComponentVec.end(); it++)
		{
			(*it)->onRepeat();
		}
		std::remove(mServerComponentVec.begin(), mServerComponentVec.end(), (void *)NULL);
	}
}

void ServerManager::addComponent(ServerComponent *component)
{
	mServerComponentVec.push_back(component);
}

void ServerManager::removeComponent(ServerComponent *component)
{
	std::vector<ServerComponent *>::iterator it = std::find(mServerComponentVec.begin(),
													mServerComponentVec.end(),
													component);
	*it = NULL;
}

void ServerManager::addFD(int fd, FileDiscriptorListener &listener)
{
	mFDManager.add(fd, listener);
}

void ServerManager::removeFD(int fd)
{
	mFDManager.remove(fd);
}
