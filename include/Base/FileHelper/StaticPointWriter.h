#pragma once
#include "FastMath.h"
#include "FileWriter.h"

class StaticPointWriter : public FileWriter
{
public:
	StaticPointWriter();
	void addPoint(const P& p);
};

class StaticPointWriterO : public FileWriterO
{
public:
	arr<P> pnts;
};