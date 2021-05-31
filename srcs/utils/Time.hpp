#ifndef TIME_HPP
# define TIME_HPP

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace web
{
	struct tm timevalToTm(struct timeval time);
	long getNowTime();
	std::string getDate();
	std::string getFileTime(std::string & path);
	std::string getLastModifiedTime(std::string & path);
};

#endif
