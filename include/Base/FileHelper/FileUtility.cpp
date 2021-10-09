#include "FileUtility.h"

#include <fstream>
#include "direct.h"
#include "Utility.h"
#include <cstring>

void ReadFileToArr(const str& path, arr<str>& lines)
{
	std::ifstream f(path.data);
	if (!f)
	{
		abort();//cout << "文件不能打开" << endl;
	}
	while (!f.is_open())
	{
		f.open(path.data);
	}

	while (!f.eof())
	{
		str line;
		getline(f, line.data);
		lines += line;
	}
}

void ArrToFile(const arr<str>& lines, const str& path)
{
	std::ofstream f(path.data, std::ios::out);
	for (int i = 0; i < lines.size(); i++)
	{
		f << lines[i]<<std::endl;
	}
	f.close();
}

//### PathGodClass
PathGodClass& PathGodClass::GetInstance()
{
	static PathGodClass ins;
	return ins;
}

str PathGodClass::GetCurrentPath()
{
	char cwd[MAX_PATH_LENGTH];
	_getcwd(cwd, MAX_PATH_LENGTH);

	std::string data(cwd);
	str re(data);
	return re;
}

void PathGodClass::AutoConfig()
{
	str cwd = GetCurrentPath();
	int inx1 = cwd.tailInxOf("ParticleToy");
	mainPath = cwd.range(0, inx1);
}
//### PathGodClass