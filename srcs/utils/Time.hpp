#ifndef TIME_HPP
# define TIME_HPP

#include <time.h>
#include <sys/time.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

namespace web
{
	struct tm timevalToTm(struct timeval time);
	long getNowTime();
	std::string getDate();
	std::string getLastModifiedTime(std::string & path);
	std::string getFileTime(std::string path);
};

#endif
