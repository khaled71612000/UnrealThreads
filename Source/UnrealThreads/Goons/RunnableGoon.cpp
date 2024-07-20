#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority) : ThreadPriority(InPriority), PrimeCounter(0) {}  // Initialize PrimeCounter to 0


RunnableGoon::~RunnableGoon()
{
	UE_LOG(LogTemp, Warning, TEXT("RunnableGoon Destructor called!"));
}

//Initialization(FThreadSafeCounter StopTaskCounter;) : The counter is initialized, typically starting at 0.
//Checking the Stopping Condition(while (StopTaskCounter.GetValue() == 0)) : The thread's main loop runs while the counter value is 0.
//Signaling the Thread to Stop(StopTaskCounter.Increment()) : When you want to stop the thread, you increment the counter.This changes the value from 0 to 1, causing the loop condition to fail and the thread to exit.

bool RunnableGoon::Init()
{
	return true;
}

uint32 RunnableGoon::Run()
{
	{
		// Lock the critical section for logging
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Thread started with priority: %d"), static_cast<int32>(ThreadPriority));
	}
	while (StopTaskCounter.GetValue() == 0)
	{
		FindPrimes();
		FPlatformProcess::Sleep(0.05f); // Reduced sleep interval for frequent stop checks
		UE_LOG(LogTemp, Warning, TEXT("Running in a separate thread!"));
	}

	{
		// Lock the critical section for logging
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Thread exiting Run method!"));
	}	
	
	LogPrimeNumbers();

	return 0;
}

void RunnableGoon::Stop()
{
	StopTaskCounter.Increment();
	{
		// Lock the critical section for logging
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Stop method called!"));
	}
}

bool RunnableGoon::IsPrime(int32 Number)
{
	if (Number <= 1) return false;
	if (Number <= 3) return true;

	if (Number % 2 == 0 || Number % 3 == 0) return false;

	for (int32 i = 5; i * i <= Number; i += 6)
	{
		if (Number % i == 0 || Number % (i + 2) == 0)
			return false;
	}
	return true;
}

void RunnableGoon::FindPrimes()
{
	int32 PrimesFound = 0;
	for (int32 i = 2; i < 10000; ++i)
	{
		if (StopTaskCounter.GetValue() != 0) // Check stop condition
		{
			break;
		}
		if (IsPrime(i))
		{
			PrimeCounter++;
			PrimesFound++;

			// Protect the shared data structure with a critical section
			{
				FScopeLock Lock(&CriticalSection);
				PrimeNumbers.Add(i);
			}
		}
	}

	{
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Exiting FindPrimes method with %d primes found!"), PrimesFound);
	}
}

void RunnableGoon::LogPrimeNumbers()
{

	//// Decrement the counter atomically
	//FPlatformAtomics::InterlockedDecrement(&AtomicCounter);
	//// Exchange (set) a value atomically and return the old value
	//int32 OldValue = FPlatformAtomics::InterlockedExchange(&AtomicCounter, 10);
	// Protect the shared data structure with a read lock
	{
		//FReadScopeLock ReadLock(ReadWriteLock);
		//for (int32 Prime : PrimeNumbers)
		//{
		//	{
		//		// Lock the critical section for logging
		//		FScopeLock Lock(&CriticalSection);
		//		UE_LOG(LogTemp, Warning, TEXT("Prime number: %d"), Prime);
		//	}
		//}
	}

	// Protect the shared data structure with a critical section
	//{
	//	FScopeLock Lock(&CriticalSection);
	//	for (int32 Prime : PrimeNumbers)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Prime number: %d"), Prime);
	//	}
	//}

	// Increment the counter atomically
	//FPlatformAtomics::InterlockedIncrement(&AtomicCounter);
	//UE_LOG(LogTemp, Warning, TEXT("Total number of primes found: %d"), AtomicCounter);
	UE_LOG(LogTemp, Warning, TEXT("Total number of primes found: %d"), PrimeCounter.Load());

}