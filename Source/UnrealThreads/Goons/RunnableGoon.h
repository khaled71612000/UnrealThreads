#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAtomics.h"
#include "Containers/Queue.h" // Include TQueue

class UNREALTHREADS_API RunnableGoon : public FRunnable
{
public:
	RunnableGoon(EThreadPriority InPriority, TQueue<int32, EQueueMode::Mpsc>& InQueue, FCriticalSection& InQueueCriticalSection, int32 CoreAffinity);
	virtual ~RunnableGoon();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter StopTaskCounter;
	EThreadPriority ThreadPriority;
	FCriticalSection CriticalSection;
	TArray<int32> PrimeNumbers;
	TAtomic<int32> PrimeCounter;

	TQueue<int32, EQueueMode::Mpsc>& Queue; // Reference to the thread-safe queue
	FCriticalSection& QueueCriticalSection; // Reference to the critical section for queue

	int32 CoreAffinity; // Core affinity for this thread

	bool IsPrime(int32 Number);
	void FindPrimes();
	void LogPrimeNumbers();
};
