#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority, TSharedPtr<TPromise<int32>> InPrimePromise)
	: ThreadPriority(InPriority), PrimeCounter(0), PrimePromise(InPrimePromise) {}

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
	{
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Thread started with priority: %d"), static_cast<int32>(ThreadPriority));
	}

	while (StopTaskCounter.GetValue() == 0)
	{
		FindPrimes();
		FPlatformProcess::Sleep(0.05f);
		UE_LOG(LogTemp, Warning, TEXT("Running in a separate thread!"));
	}

	{
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
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Stop method called!"));
	}

	if (PrimePromise.IsValid())
	{
		FScopeLock Lock(&CriticalSection); // Ensure thread-safe access
		PrimePromise->SetValue(PrimeCounter.Load());
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

			FScopeLock Lock(&CriticalSection);
			PrimeNumbers.Add(i);
		}
	}

	FScopeLock Lock(&CriticalSection);
	UE_LOG(LogTemp, Warning, TEXT("Exiting FindPrimes method with %d primes found!"), PrimesFound);
}

void RunnableGoon::LogPrimeNumbers()
{
	UE_LOG(LogTemp, Warning, TEXT("Total number of primes found: %d"), PrimeCounter.Load());
}
