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
	void println(const std::string tag, const std::string msg);
	void println(const std::string tag, const char * msg);
	std::ostream &print(const std::string tag);
};

#endif
