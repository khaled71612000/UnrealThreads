#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "ITask.h"

class PrimeCalculationTask : public ITask
{
public:
	PrimeCalculationTask(int32 InNumber, TQueue<int32, EQueueMode::Mpsc>& InQueue, FCriticalSection& InQueueCriticalSection);
	virtual void Execute() override;

private:
	int32 Number;
	TQueue<int32, EQueueMode::Mpsc>& Queue;
	FCriticalSection& QueueCriticalSection;

	bool IsPrime(int32 Num);
};
