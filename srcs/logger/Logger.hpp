#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <string>
# include <time.h>
# include <sys/time.h>
# include <unistd.h>
# include "utils/Utils.hpp"

namespace logger
{
	void print(const std::string tag, const std::string msg);
	void print(const std::string tag, const char * msg);
};

#endif
