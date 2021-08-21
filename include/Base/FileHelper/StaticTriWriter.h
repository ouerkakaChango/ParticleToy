#pragma once
#include "FileWriter.h"
#include "FastGeo.h"

class StaticTriWriter : public FileWriter
{
public:
	StaticTriWriter();
	void Load(arr<Tri>* triArr);
	void Write(const str& outPath);
};

class StaticTriWriterO : public FileWriterO
{
public:
	StaticTriWriterO(FileWriter* y_);
	void Write(FileWriterI* i, const str& filePath) override;
};

class StaticTriWriterI : public FileWriterI
{
public:
	arr<Tri>* triArr=nullptr;
};