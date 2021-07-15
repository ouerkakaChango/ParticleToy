#include "StaticPointWriter.h"

StaticPointWriter::StaticPointWriter()
{
	i += new FileWriterI;
	o += new StaticPointWriterO;
}

void StaticPointWriter::addPoint(const P& p)
{
	auto to = Cast<StaticPointWriterO*>(o[0]);
	to->pnts += p;
}