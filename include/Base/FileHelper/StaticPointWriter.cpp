#include "StaticPointWriter.h"

#include <iostream>
#include <fstream>
using std::cout;

//### StaticPointWriter
StaticPointWriter::StaticPointWriter()
{
	i += new StaticPointWriterI(this);
	o += new StaticPointWriterO;
}

void StaticPointWriter::addPoint(const P& p)
{
	auto to = Cast<StaticPointWriterO*>(o[0]);
	to->pnts += p;
}

void StaticPointWriter::Write(const str& outPath)
{
	auto ti = Cast<StaticPointWriterI*>(i[0]);
	auto to = Cast<StaticPointWriterO*>(o[0]);
	ti->Write(to, outPath);
}
//### StaticPointWriter

//### StaticPointWriterI
StaticPointWriterI::StaticPointWriterI(FileWriter* y_):FileWriterI(y_)
{


}

void StaticPointWriterI::Write(FileWriterO* o, const str& filePath)
{
	if (typeStr(*o) == "class StaticPointWriterO")
	{
		auto to = Cast<StaticPointWriterO*>(o);

		std::cout << "Writing File...\n";
		std::ofstream f(filePath.data, std::ios::out);
		for (int inx = 0; inx < to->pnts.size(); inx++)
		{
			auto& p = to->pnts[inx];

			if (y->wMode == WriteMode_Houdini)
			{
				double tt = p.y;
				p.y = p.z;
				p.z = tt;
			}
			f << to->pnts[inx].ToStr(6)<< "\n";
		}
		std::cout << "File write done at " << filePath << "\n";
	}
}
//### StaticPointWriterI