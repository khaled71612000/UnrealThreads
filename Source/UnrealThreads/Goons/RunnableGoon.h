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
	RunnableGoon(EThreadPriority InPriority);
	virtual ~RunnableGoon();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter StopTaskCounter;
	EThreadPriority ThreadPriority;

	FCriticalSection CriticalSection;
	TQueue<int32, EQueueMode::Spsc> PrimeQueue; // Thread-safe queue for prime numbers

	bool IsPrime(int32 Number);
	TAtomic<int32> PrimeCounter;
	void FindPrimes();
	void LogPrimeNumbers();
};
