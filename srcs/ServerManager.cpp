#include "ServerManager.hpp"

const std::string ServerManager::TAG = "ServerManager";

ServerManager::ServerManager()
{

}

ServerManager::~ServerManager()
{
	for (componentIter it = mServerComponentVec.begin(); it < mServerComponentVec.end(); it++)
	{
		if (*it)
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
			if (*it)
				(*it)->onRepeat();
		}
		componentIter it = std::remove(mServerComponentVec.begin(), mServerComponentVec.end(), (void *)NULL);
		if (it != mServerComponentVec.end())
			mServerComponentVec.erase(it);
	}
}

void ServerManager::addComponent(ServerComponent *component)
{
	mServerComponentVec.push_back(component);
}

void ServerManager::removeComponent(ServerComponent *component)
{
	for (componentIter it = mServerComponentVec.begin(); it < mServerComponentVec.end(); it++)
	{
		if (*it == component)
			*it = NULL;
	}
}

void ServerManager::addFD(int fd, FileDiscriptorListener &listener)
{
	mFDManager.add(fd, listener);
}

void ServerManager::removeFD(int fd)
{
	mFDManager.remove(fd);
}
