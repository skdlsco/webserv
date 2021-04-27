#ifndef FILE_DISCRIPTOR_SET_HPP
# define FILE_DISCRIPTOR_SET_HPP

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
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
	public:
		static const std::string TAG;
		static const int FD_MAX = 1024;

		FileDiscriptorManager();
		virtual ~FileDiscriptorManager();

		void add(int fd, FileDiscriptorListener &listener);
		void remove(int fd);
		bool select();
};

#endif
