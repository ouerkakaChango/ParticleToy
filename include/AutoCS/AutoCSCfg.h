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
		map<str,arr<str>> blocks;

	private:
		void DoBlockChange(arr<str>& lines);
		void DoValMap(arr<str>& lines);
	};
}