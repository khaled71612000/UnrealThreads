#pragma once

class ITask
{
public:
	virtual ~ITask() {}
	virtual void Execute() = 0;
};
