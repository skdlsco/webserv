#ifndef FILE_DISCRIPTOR_SET_HPP
# define FILE_DISCRIPTOR_SET_HPP

#ifndef FD_SETSIZE
# define FD_SETSIZE 1024
#endif
#ifndef NFDBITS
# define NFDBITS 32
#endif


#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include "FileDiscriptorListener.hpp"
#include <vector>
#include <string>

class FileDiscriptorManager
{
	private:
		fd_set mFDSet;
		std::vector<FileDiscriptorListener *> mListenerVec;

		FileDiscriptorManager(FileDiscriptorManager const & copy);
		FileDiscriptorManager & operator=(FileDiscriptorManager const & copy);

		void fdZero();
		void fdSet(int fd);
		void fdClr(int fd);
		bool isFDSet(int fd, fd_set * fdSet);
		bool isFDOverflow(int fd);
	public:
		static std::string const TAG;
		static const int FD_MAX = FD_SETSIZE;
		FileDiscriptorManager();
		virtual ~FileDiscriptorManager();

		void add(int fd, FileDiscriptorListener &listener);
		void remove(int fd);
		bool select();
};

#endif
