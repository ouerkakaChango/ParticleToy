#pragma once

#include "Taiji.h"

enum WriteMode
{
	WriteMode_Unset,
	WriteMode_Houdini,
};

class FileWriter 
{
	THISY(FileWriter)

	virtual void Write(const str& outPath) = 0; 
	inline void SetWriteMode(WriteMode wMode_) { wMode = wMode_; };
	WriteMode wMode;
};

class FileWriterO;
class FileWriterI : public ClassI
{
public:
	FileWriterI(FileWriter* y_);
	virtual void Write(FileWriterO* o, const str& filePath) = 0;
	FileWriter* y = nullptr;
};

class FileWriterO : public ClassO
{

};