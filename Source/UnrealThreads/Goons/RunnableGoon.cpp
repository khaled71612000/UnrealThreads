#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority) : ThreadPriority(InPriority) // Initialize the thread priority
{
}

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
	UE_LOG(LogTemp, Warning, TEXT("Thread started with priority: %d"), static_cast<int32>(ThreadPriority));

	while (StopTaskCounter.GetValue() == 0)
	{
		FindPrimes();
		FPlatformProcess::Sleep(0.1f); // Reduced sleep interval for frequent stop checks
		UE_LOG(LogTemp, Warning, TEXT("Running in a separate thread!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Thread exiting Run method!"));
	return 0;
}

void RunnableGoon::Stop()
{
	StopTaskCounter.Increment();
	UE_LOG(LogTemp, Warning, TEXT("Stop method called!"));
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
	for (int32 i = 2; i < 10000; ++i)
	{
		if (StopTaskCounter.GetValue() != 0) // Check stop condition
		{
			break;
		}
		if (IsPrime(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("Prime number: %d"), i);
			FPlatformProcess::Sleep(0.1f); // Reduced sleep interval for frequent stop checks
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Exiting FindPrimes method!"));
}