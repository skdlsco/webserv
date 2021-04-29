#include "FileDiscriptorManager.hpp"

const std::string FileDiscriptorManager::TAG = "FileDiscriptorManager";

FileDiscriptorManager::FileDiscriptorManager()
{
	FD_ZERO(&mFDSet); // TODO FD_ZERO
	mListenerVec = std::vector<FileDiscriptorListener *>(FD_MAX);
}

FileDiscriptorManager::~FileDiscriptorManager()
{
	mListenerVec.clear();
}

void FileDiscriptorManager::add(int fd, FileDiscriptorListener &listener)
{
	FD_SET(fd, &mFDSet); // TODO FD_SET
	mListenerVec[fd] = &listener;
}

void FileDiscriptorManager::remove(int fd)
{
	FD_CLR(fd, &mFDSet); // TODO FD_CLR
	mListenerVec[fd] = NULL;
}

bool FileDiscriptorManager::select()
{
	fd_set readset = mFDSet;
	fd_set writeset = mFDSet;
	fd_set exceptset = mFDSet;

	if (::select(FD_MAX, &readset, &writeset, &exceptset, NULL))
	{
		for (int i = 0; i < FD_MAX; i++)
		{
			if (FD_ISSET(i, &readset))
				mListenerVec[i]->onReadSet();
			if (FD_ISSET(i, &writeset))
				mListenerVec[i]->onWriteSet();
			if (FD_ISSET(i, &exceptset))
				mListenerVec[i]->onExceptSet();
		}
	}
	return (false);
}
