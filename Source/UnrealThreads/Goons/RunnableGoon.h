#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAtomics.h"
#include "Templates/Atomic.h"
#include "HAL/PlatformProcess.h"  // Include semaphore header
#include "../../../../../../../Source/Runtime/Core/Public/Windows/WindowsSemaphore.h"

class UNREALTHREADS_API RunnableGoon : public FRunnable
{
public:
	RunnableGoon(EThreadPriority InPriority, FWindowsSemaphore* InSemaphore);
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

	FWindowsSemaphore* Semaphore;  // Semaphore for controlling access

	bool IsPrime(int32 Number);
	void FindPrimes();
	void LogPrimeNumbers();
};
