#pragma once
#include<cstdlib>
#include<ctime>
#include "Container.h"

class Timer
{
public:
	void Start();
	double Record();
	str Record_FormatStr();
	void LogRecord();

	clock_t start;
};