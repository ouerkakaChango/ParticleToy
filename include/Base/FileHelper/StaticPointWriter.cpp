#include "StaticPointWriter.h"

#include "WriterUtility.h"

#include <iostream>
#include <fstream>
using std::cout;

//### StaticPointWriter
StaticPointWriter::StaticPointWriter()
{
	i += new StaticPointWriterI;
	o += new StaticPointWriterO(this);
}

void StaticPointWriter::addPoint(const P& p)
{
	auto ti = Cast<StaticPointWriterI*>(i[0]);
	ti->pnts += p;
}

void StaticPointWriter::Write(const str& outPath)
{
	auto ti = Cast<StaticPointWriterI*>(i[0]);
	auto to = Cast<StaticPointWriterO*>(o[0]);
	to->Write(ti, outPath);
}
//### StaticPointWriter

//### StaticPointWriterO
StaticPointWriterO::StaticPointWriterO(FileWriter* y_):FileWriterO(y_)
{


}

void StaticPointWriterO::Write(FileWriterI* i, const str& filePath)
{
	if (typeStr(*i) == "class StaticPointWriterI")
	{
		auto ti = Cast<StaticPointWriterI*>(i);

		std::cout << "Writing File...\n";
		std::ofstream f(filePath.data, std::ios::out);
		for (int inx = 0; inx < ti->pnts.size(); inx++)
		{
			auto p = ti->pnts[inx];

			p = TransWriteFormat(p,y->wMode);

			f << p.ToStr(6)<< "\n";
		}
		std::cout << "File write done at " << filePath << "\n";
	}
}
//### StaticPointWriterI