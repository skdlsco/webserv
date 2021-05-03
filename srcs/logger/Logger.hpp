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
	void println(std::string const tag, std::string const msg);
	void println(std::string const tag, const char * msg);
	std::ostream &print(std::string const tag);
};

#endif
