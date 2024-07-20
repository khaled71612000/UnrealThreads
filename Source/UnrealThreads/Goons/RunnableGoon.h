#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAtomics.h"
//#include "HAL/ReadWriteLock.h" // Ensure this header is included
#include "Templates/Atomic.h"

//its not a uclass with a generated body and include files
class UNREALTHREADS_API RunnableGoon : public FRunnable
{

public:
	RunnableGoon(EThreadPriority InPriority);
	virtual ~RunnableGoon();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;


private:

	//FThreadSafeCounter used to safely signal and check the stopping condition of the thread across multiple threads.It provides an atomic way to increment, decrement, and retrieve the counter value, ensuring thread safety
	FThreadSafeCounter StopTaskCounter;
	EThreadPriority ThreadPriority; // Store the thread priority

	FCriticalSection CriticalSection; // Critical section for synchronization
	//FReadWriteLock ReadWriteLock; // For shared data structure
	TArray<int32> PrimeNumbers; // Shared data structure

	// Method to check if a number is prime
	bool IsPrime(int32 Number);
	TAtomic<int32> PrimeCounter; // Atomic counter for counting primes found

	// Atomic integer variable
	int32 AtomicCounter = 0;
	// Simulate a practical task of finding prime numbers
	void FindPrimes();
	void LogPrimeNumbers();
};
