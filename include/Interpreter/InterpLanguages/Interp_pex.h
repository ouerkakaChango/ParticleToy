#pragma once

#include "../Interpreter.h"

namespace Interp
{
	class InterpreterO_pex : public InterpreterO
	{
	public:
		void Load(const str& path) override;
	};
}