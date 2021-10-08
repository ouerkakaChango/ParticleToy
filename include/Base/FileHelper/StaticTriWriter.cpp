#include "StaticTriWriter.h"

#include "WriterUtility.h"
#include <iostream>
#include <fstream>
using std::cout;

//### StaticTriWriter
StaticTriWriter::StaticTriWriter()
{
	i += new StaticTriWriterI;
	o += new StaticTriWriterO(this);
}

void StaticTriWriter::Load(arr<Tri>* triArr)
{
	auto ti = Cast<StaticTriWriterI*>(i[0]);
	ti->triArr = triArr;
}

void StaticTriWriter::Write(const str& outPath)
{
	auto ti = Cast<StaticTriWriterI*>(i[0]);
	auto to = Cast<StaticTriWriterO*>(o[0]);
	to->Write(ti, outPath);
}
//### StaticTriWriter

//### StaticTriWriterO
StaticTriWriterO::StaticTriWriterO(FileWriter* y_) :FileWriterO(y_)
{


}

void StaticTriWriterO::Write(FileWriterI* i, const str& filePath)
{
	if (typeStr(*i) == "class StaticTriWriterI")
	{
		auto ti = Cast<StaticTriWriterI*>(i);

		std::cout << "Writing File...\n";
		std::ofstream f(filePath.data, std::ios::out);
		for (int inx = 0; inx < ti->triArr->size(); inx++)
		{
			auto tri = (*ti->triArr)[inx];

			tri = TransWriteFormat(tri, y->wMode);
			f << tri.p1.ToStr(6) <<" "<< tri.p2.ToStr(6)<< " " << tri.p3.ToStr(6) << "\n";
			//break;
		}
		f.close();
		std::cout << "File write done at " << filePath << "\n";
	}
}
//### StaticTriWriterI