#pragma once

class TickInfo
{
public:
	class PhysicSolver* physic;
	int F;
};

typedef std::function<void(TickInfo&)> TickFunc;