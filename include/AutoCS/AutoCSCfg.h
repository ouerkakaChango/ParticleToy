#pragma once
#include "Container.h"
namespace AutoCS
{
	class AutoCSCfg
	{
	public:
		AutoCSCfg(str cfgFile);
		void Generate(str templateFile, str outFile);

		map<str, str> valMap;
	};
}