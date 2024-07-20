#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAtomics.h"
#include "Templates/Atomic.h"
#include "Async/Future.h"

class UNREALTHREADS_API RunnableGoon : public FRunnable
{
public:
	RunnableGoon(EThreadPriority InPriority, TSharedPtr<TPromise<int32>> InPrimePromise);
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

	TSharedPtr<TPromise<int32>> PrimePromise;

	bool IsPrime(int32 Number);
	void FindPrimes();
	void LogPrimeNumbers();
};
