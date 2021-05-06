#include "Logger.hpp"

static void printTime()
{
	char buffer[18];
	struct timeval time;
	struct tm t;

	gettimeofday(&time, NULL);
	t = utils::timavalToTm(time);
	strftime(buffer, 18, "%x %X", &t);
	buffer[17] = '\0';
	std::cout << buffer;
}

void logger::println(std::string const tag, std::string const msg)
{
	printTime();
	std::cout << " [" << tag << "]" << ": " << msg << std::endl;
}

void logger::println(std::string const tag, const char *msg)
{
	printTime();
	std::cout << " [" << tag << "]" << ": " << msg << std::endl;
}

std::ostream &logger::print(std::string const tag)
{
	printTime();
	return (std::cout << " [" << tag << "]" << ": ");
}
