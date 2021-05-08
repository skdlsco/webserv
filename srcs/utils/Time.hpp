#ifndef TIME_HPP
# define TIME_HPP

# include <time.h>
# include <sys/time.h>
# include <unistd.h>

namespace web
{
	struct tm timevalToTm(struct timeval time);
};

#endif
