#include "AutoCSCfg.h"
#include "FileHelper/FileUtility.h"
namespace AutoCS
{
	AutoCSCfg::AutoCSCfg(str cfgFile)
	{
		arr<str> lines;
		FileToArr(cfgFile, lines);
		for (int i = 0; i < lines.size(); i++)
		{
			str s = lines[i];
			arr<str> words = s.pieces(' ');
			valMap[words[0]] = words[1];
		}
	}

	void AutoCSCfg::Generate(str templateFile, str outFile)
	{
		arr<str> lines;
		FileToArr(templateFile, lines);
		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i].Has("(@"))
			{
				str line = lines[i];
				auto begins = line.allIndices("(@");
				//!!! 现在没支持嵌套语法
				for (int i1 = 0; i1 < begins.size(); i1++)
				{
					int begin = begins[i1];
					int end = line.inxOf(')', begin);
					str key = line.range(begin+2, end-1);
					if (valMap.Has(key))
					{
						line = line.replace(begin, key.size()+3, valMap[key]);
					}
				}
				lines[i] = line;
			}
		}
		ArrToFile(lines, outFile);
	}

}