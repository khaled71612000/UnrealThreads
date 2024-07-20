#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority, FWindowsSemaphore* InSemaphore)
	: ThreadPriority(InPriority), PrimeCounter(0), Semaphore(InSemaphore) {}

RunnableGoon::~RunnableGoon()
{
	UE_LOG(LogTemp, Warning, TEXT("RunnableGoon Destructor called!"));
}

bool RunnableGoon::Init()
{
	return true;
}

uint32 RunnableGoon::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
		Semaphore->Acquire();  // Acquire the semaphore
		FindPrimes();
		Semaphore->Release();  // Release the semaphore

		FPlatformProcess::Sleep(0.05f);
	}

	LogPrimeNumbers();

	return 0;
}

void RunnableGoon::Stop()
{
	StopTaskCounter.Increment();
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

			FScopeLock Lock(&CriticalSection);
			PrimeNumbers.Add(i);
		}
	}

	FScopeLock Lock(&CriticalSection);
	UE_LOG(LogTemp, Warning, TEXT("Thread %d"), FPlatformTLS::GetCurrentThreadId());
}

void RunnableGoon::LogPrimeNumbers()
{
	UE_LOG(LogTemp, Warning, TEXT("Total number of primes found: %d"), PrimeCounter.Load());
}
