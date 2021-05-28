#include "Time.hpp"

static bool isLeapYear(int year)
{
	int now = year;

	return ((!(now % 4) && (now % 100)) || !(now % 400));
}

static int getYearLength(int year)
{
	if (isLeapYear(year))
		return (366);
	return (365);
}

static int getMonthLength(int month, int year)
{
	static const int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 0 || month > 11)
		return (31);
	if (month == 1 && isLeapYear(year))
		return (29);
	return (monthDays[month]);
}

struct tm web::timevalToTm(struct timeval time)
{
	struct tm result;
	int year = 0;
	int days = 0;

	result.tm_sec = time.tv_sec % 60;
	time.tv_sec /= 60;
	result.tm_min = time.tv_sec % 60;
	time.tv_sec /= 60;

	result.tm_hour = time.tv_sec % 24;
	time.tv_sec /= 24;
	result.tm_wday = ((time.tv_sec + 4) % 7);

	while((days += getYearLength(year + 70)) <= time.tv_sec)
		year++;
	result.tm_year = year + 70;
	days -= getYearLength(year + 70);
	time.tv_sec -= days;
	result.tm_yday = time.tv_sec;
	for (int month = 0; month < 12; month++)
	{
		int monthDays = getMonthLength(month, year);

		if (time.tv_sec >= monthDays)
			time.tv_sec -= monthDays;
		else
		{
			result.tm_mon = month;
			break ;
		}
	}
	result.tm_mday = time.tv_sec;
	return (result);
}

long web::getNowTime()
{
	struct timeval nowTime;

	gettimeofday(&nowTime, NULL);
	return (nowTime.tv_sec);
}

std::string web::getDate()
{
	char buffer[32];

	bzero(buffer, sizeof(buffer));
	struct timeval time;
	struct tm t;

	gettimeofday(&time, NULL);
	t = web::timevalToTm(time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %X GMT", &t);
	return (buffer);
}

std::string web::getLastModifiedTime(std::string & path)
{
	struct stat sb;
	char buffer[32];

	bzero(buffer, sizeof(buffer));
	stat(path.c_str(), &sb);

	struct timeval time;
	struct tm t;

	time.tv_sec = sb.st_mtim.tv_sec;
	time.tv_usec = sb.st_mtim.tv_nsec / 1000;

	t = web::timevalToTm(time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %X GMT", &t);
	return (buffer);
}

/* for autoindex */
std::string web::getFileTime()
{
	char buffer[32];
	bzero(buffer, sizeof(buffer));

	struct timeval time;
	struct tm t;
	
	gettimeofday(&time, NULL);
	t = web::timevalToTm(time);
	strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", &t);
	return (buffer);
}