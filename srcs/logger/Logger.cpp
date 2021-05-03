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

void logger::println(const std::string tag, const std::string msg)
{
	printTime();
	std::cout << " [" << tag << "]" << ": " << msg << std::endl;
}

void logger::println(const std::string tag, const char *msg)
{
	printTime();
	std::cout << " [" << tag << "]" << ": " << msg << std::endl;
}

std::ostream &logger::print(const std::string tag)
{
	printTime();
	return (std::cout << " [" << tag << "]" << ": ");
}
