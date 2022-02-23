#include "AutoCSCommonDef.h"
namespace AutoCS
{
	void ParseTask(const arr<str>& lines, arr<str>& templates, arr<str>& outs, arr<str>& cfgs)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			if (lines[i].Has("templates:"))
			{
				i++;
				while (lines[i].size() != 0)
				{
					templates += lines[i];
					i++;
				}
			}

			if (lines[i].Has("outs:"))
			{
				i++;
				while (lines[i].size() != 0)
				{
					outs += lines[i];
					i++;
				}
			}

			if (lines[i].Has("configs:"))
			{
				i++;
				while (lines[i].size() != 0)
				{
					cfgs += lines[i];
					i++;
				}
			}
		}
	}

}