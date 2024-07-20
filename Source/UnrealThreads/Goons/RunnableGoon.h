#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAtomics.h"
#include "Containers/Queue.h"
#include "Templates/Atomic.h"

class UNREALTHREADS_API RunnableGoon : public FRunnable
{
public:
	RunnableGoon(EThreadPriority InPriority, TQueue<int32, EQueueMode::Mpsc>* InPrimeQueue);
	virtual ~RunnableGoon();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	FEvent* PrimeFoundEvent;

private:
	FThreadSafeCounter StopTaskCounter;
	EThreadPriority ThreadPriority;

	FCriticalSection CriticalSection;
	TQueue<int32, EQueueMode::Mpsc>* PrimeQueue; // Thread-safe queue for prime numbers

	bool IsPrime(int32 Number);
	TAtomic<int32> PrimeCounter;
	void FindPrimes();
};
