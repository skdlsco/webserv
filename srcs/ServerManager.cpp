#include "ServerManager.hpp"

std::string const ServerManager::TAG = "ServerManager";

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
				(*it)->onRepeat(makeTimeOut());
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

struct timeval ServerManager::makeTimeOut()
{
	struct timeval timeOut;

	timeOut.tv_sec = 10;
	timeOut.tv_usec = 0;
	return (timeOut);
}