#ifndef UTILS_HPP
# define UTILS_HPP

# include <time.h>
# include <sys/time.h>
# include <unistd.h>

namespace utils
{
	struct tm timavalToTm(struct timeval time);
};

#endif
