#include "Utils.hpp"

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

struct tm utils::timavalToTm(struct timeval time)
{
	struct tm result;
	int year = 0;
	int days = 0;

	result.tm_sec = time.tv_sec % 60;
	time.tv_sec /= 60;
	result.tm_min = time.tv_sec % 60;
	time.tv_sec /= 60;
	
	//for UTC+9
	time.tv_sec += 9;
	result.tm_hour = time.tv_sec % 24;
	time.tv_sec /= 24;
	result.tm_wday = ((time.tv_sec + 4) % 7);

	while((days += getYearLength(year + 1970)) <= time.tv_sec)
		year++;
	result.tm_year = year + 1970;
	days -= getYearLength(year + 1970);
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
