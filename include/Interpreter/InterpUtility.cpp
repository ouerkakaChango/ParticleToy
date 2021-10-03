#include "InterpUtility.h"

namespace Interp
{
	bool CheckType(const str& path, LanguageType type)
	{
		if (type == LanguageType_pex)
		{
			return path.endWith(".pex");
		}
		else if (type == LanguageType_python)
		{
			return path.endWith(".py");
		}
		else
		{
			return false;
		}
	}

	LanguageType GetLanguageType(const str& path)
	{
		if (path.endWith(".pex"))
		{
			return LanguageType_pex;
		}
		else if (path.endWith(".py"))
		{
			return LanguageType_python;
		}
	}

	bool ValidFile(str fileStr, str& fileName)
	{
		int len = fileStr.size();
		if (fileStr[0] == '<' && fileStr[len - 1] == '>')
		{
			fileName = fileStr.range(1, len - 2);
			return true;
		}
		return false;
	}

	//((,)),(((,)))
	//12,10 , 123,210
	arr<int2> Level1Brackets(const str& s)
	{
		int num = 0;
		arr<int2> re;
		int2 tt;
		for (int i = 0; i < s.size(); i++)
		{
			char c = s[i];
			if (c == '(')
			{
				num += 1;
				if (num == 1)
				{
					tt.x = i;
				}
			}
			else if (c == ')')
			{
				num -= 1;
				if (num == 0)
				{
					tt.y = i;
					re += tt;
				}
			}	
		}
		return re;
	}

	arr<int> FilterRanges(const arr<int>& intervals, const arr<int2>& ranges)
	{
		arr<int> re;
		if (ranges.size() == 0)
		{
			return intervals;
		}
		else
		{
			int rInx = 0;
			int2 tRange = ranges[rInx];
			int i = 0;
			while (i < intervals.size())
			{
				int tt = intervals[i];
				if (tt < tRange.x)
				{
					re += tt;
				}
				if (tt > tRange.x && tt < tRange.y)
				{
					//do nothing
				}
				else if (tt > tRange.y)
				{
					rInx += 1;
					if (rInx >= ranges.size())
					{
						re += tt;
						i += 1;
						break;
					}
					else
					{
						tRange = ranges[rInx];
						i -= 1;
					}
				}
				i += 1;
			}

			for (; i < intervals.size(); i++)
			{
				re += intervals[i];
			}
		}

		return re;
	}

	arr<str> GetParams(const str& s)
	{
		arr<int> intervals = s.indicesOf(',');
		arr<int2> ranges = Level1Brackets(s);
		intervals = FilterRanges(intervals, ranges);
		return s.pieces(intervals);
	}
}