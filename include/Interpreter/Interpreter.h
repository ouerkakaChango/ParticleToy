#pragma once

#include "InterpUtility.h"
#include "Taiji.h"

namespace Interp
{
	class Interpreter
	{
		THISY(Interpreter)
		Interpreter(LanguageType srcType_, LanguageType dstType_);
		Interpreter(const str& srcPath_, const str& dstPath_);

		void Interp();

	protected:
		void Init();
		bool LoadSrc();
		bool CheckTypeValid();

		LanguageType srcType, dstType;
		str srcPath, dstPath;
	};

	//无幻不起真，虚为启
	//InterpreterO 处理srcLanguage
	class InterpreterO : public ClassO
	{
		public:
			virtual void Load(const str& path) {};
	};

	class InterpreterI : public ClassI
	{
	public:
		virtual void Parse(const str& path) {};
	};
}