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

class StaticPointWriterI : public FileWriterI
{
public:
	StaticPointWriterI(FileWriter* y_);
	void Write(FileWriterO* o, const str& filePath) override;
};

class StaticPointWriterO : public FileWriterO
{
public:
	arr<P> pnts;
};