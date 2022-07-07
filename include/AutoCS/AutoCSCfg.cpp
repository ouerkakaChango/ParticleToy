#include "AutoCSCfg.h"
#include "FileHelper/FileUtility.h"
namespace AutoCS
{
	AutoCSCfg::AutoCSCfg(str cfgFile)
	{
		arr<str> lines;
		FileToArr(cfgFile, lines);
		NiceSpaceBar(lines);
		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i].Has("valMaps"))
			{
				i += 1;
				while (i<lines.size() && lines[i].size() > 0)
				{
					str s = lines[i];
					if (!s.beginWith("//"))
					{
						arr<str> words = s.pieces(' ');
						valMap[words[0]] = words[1];
					}
					i++;
				}
			}

			if (i >= lines.size())
			{
				break;
			}
			if (lines[i].Has("Blocks:"))
			{
				i++;
				arr<str> tBlock;
				str blockName;
				bool bInBlock = false;
				while (i < lines.size())
				{
					if (lines[i].beginWith("###BLOCK "))
					{
						blockName = lines[i].clip("###BLOCK ");
						bInBlock = true;
						i++;
						continue;
					}
					else if (lines[i] == "###BLOCK")
					{
						//dump tblocks to blocks
						bInBlock = false;
						blocks[blockName]= tBlock;
						tBlock.clear();
						i++;
						continue;
					}
					else if(!bInBlock && lines[i].size()>0)
					{
						break;
					}
					if (bInBlock)
					{
						tBlock += lines[i];
					}
					i++;
				}
			}
		}
	}

	void AutoCSCfg::Generate(str templateFile, str outFile)
	{
		arr<str> lines;
		FileToArr(templateFile, lines);
		NiceSpaceBar(lines);
		DoBlockChange(lines);
		DoValMap(lines);
		
		ArrToFile(lines, outFile);
	}

	void AutoCSCfg::DoBlockChange(arr<str>& lines)
	{
		int blockBegin = -1;
		int blockEnd = -1;
		str blockName;
		map<str,int2> markers;
		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i].Has("###BLOCK "))
			{
				int prefixLen = 1+ lines[i].tailInxOf("###BLOCK ");
				blockName = lines[i].clip(prefixLen);
				blockBegin = i;
				i++;
				while (i<lines.size())
				{
					if (lines[i].Has("###BLOCK"))
					{
						blockEnd = i;
						markers[blockName] = int2(blockBegin, blockEnd);
						break;
					}
					i++;
				}
			}
		}

		int offset = 0;
		int blockLen = -1;
		for (int i = 0; i < markers.keys.size(); i++)
		{
			blockName = markers.keys[i];
			blockBegin = markers.values[i].x;
			blockEnd = markers.values[i].y;
			blockLen = blockEnd - blockBegin + 1;
			//del block
			lines.delRange(offset + blockBegin, offset + blockEnd);
			//insert 
			int insertPos = offset + blockEnd + 1 - blockLen;
			lines.insertBefore(insertPos, blocks[blockName]);
			offset += blocks[blockName].size() - blockLen;
		}
	}

	void AutoCSCfg::DoValMap(arr<str>& lines)
	{
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
					str key = line.range(begin + 2, end - 1);
					if (valMap.Has(key))
					{
						line = line.replace(begin, key.size() + 3, valMap[key]);
					}
				}
				lines[i] = line;
			}
		}
	}
}