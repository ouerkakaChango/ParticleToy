#include "Timer.h"

#include<iostream>
using std::cout;
using std::endl;

void Timer::Start()
{
	start = clock();
}

double Timer::Record()
{
	clock_t end = clock();
	return (double)(end - start) / CLOCKS_PER_SEC;
}

str Timer::Record_FormatStr()
{
	str re;
	double t = Record();
	if (t < 60)
	{
		re.AddDouble(t, 3);
		re += "sec";
	}
	else
	{
		int minute = floor(t / 60);
		double sec = t - minute * 60;
		re += minute;
		re += "min ";
		re.AddDouble(sec, 3);
		re += "sec";
	}
	return re;
}

void Timer::LogRecord()
{
	str log = "Now Time:";
	log += Record_FormatStr();
	cout << log <<endl;
}