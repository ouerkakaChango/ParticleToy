#pragma once
#include "FastMath.h"
#include "FileWriter.h"

class StaticPointWriter : public FileWriter
{
public:
	StaticPointWriter();
	void addPoint(const P& p);
	void Write(const str& outPath);
};

class StaticPointWriterO : public FileWriterO
{
public:
	StaticPointWriterO(FileWriter* y_);
	void Write(FileWriterI* i, const str& filePath) override;
};

class StaticPointWriterI : public FileWriterI
{
public:
	arr<P> pnts;
};