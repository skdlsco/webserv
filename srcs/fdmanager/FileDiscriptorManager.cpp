#include "FileDiscriptorManager.hpp"

std::string const FileDiscriptorManager::TAG = "FileDiscriptorManager";

FileDiscriptorManager::FileDiscriptorManager()
{
	fdZero();
	mListenerVec = std::vector<FileDiscriptorListener *>(FD_MAX);
}

FileDiscriptorManager::~FileDiscriptorManager()
{
	mListenerVec.clear();
}

void FileDiscriptorManager::fdZero()
{
	unsigned char *it;

	it = reinterpret_cast<unsigned char *>(&mFDSet);
	for (unsigned long i = 0; i < sizeof(mFDSet); i++, it++)
		*it = 0;
}

bool FileDiscriptorManager::isFDOverflow(int fd)
{
	return (fd >= FD_MAX);
}

void FileDiscriptorManager::fdSet(int fd)
{
	if (!isFDOverflow(fd))
		mFDSet.fds_bits[(unsigned long)fd / __DARWIN_NFDBITS] |= ((__int32_t)(((unsigned long)1) << ((unsigned long)fd % __DARWIN_NFDBITS)));
}

void FileDiscriptorManager::fdClr(int fd)
{
	if (!isFDOverflow(fd))
		mFDSet.fds_bits[(unsigned long)fd / __DARWIN_NFDBITS] &= ~((__int32_t)(((unsigned long)1) << ((unsigned long)fd % __DARWIN_NFDBITS)));
}

bool FileDiscriptorManager::isFDSet(int fd, struct fd_set * fdSet)
{
	if (!isFDOverflow(fd))
		return (fdSet->fds_bits[(unsigned long)fd / __DARWIN_NFDBITS] & ((__int32_t)(((unsigned long)1) << ((unsigned long)fd % __DARWIN_NFDBITS))));
	return (false);
}

void FileDiscriptorManager::add(int fd, FileDiscriptorListener &listener)
{
	fdSet(fd);
	mListenerVec[fd] = &listener;
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

void FileDiscriptorManager::remove(int fd)
{
	fdClr(fd);
	mListenerVec[fd] = NULL;
	close(fd);
}

bool FileDiscriptorManager::select()
{
	struct timeval time;
	struct fd_set readset = mFDSet;
	struct fd_set writeset = mFDSet;
	struct fd_set exceptset = mFDSet;

	time.tv_sec = 0;
	time.tv_usec = 0;
	if (::select(FD_MAX, &readset, &writeset, &exceptset, &time))
	{
		for (int i = 0; i < FD_MAX; i++)
		{
			if (isFDSet(i, &readset))
				mListenerVec[i]->onReadSet();
			if (isFDSet(i, &writeset))
				mListenerVec[i]->onWriteSet();
			if (isFDSet(i, &exceptset))
				mListenerVec[i]->onExceptSet();
		}
	}
	return (false);
}
