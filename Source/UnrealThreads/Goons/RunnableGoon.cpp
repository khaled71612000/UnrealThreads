#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"
#include "HAL/PlatformTLS.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority, TQueue<int32, EQueueMode::Mpsc>* InPrimeQueue)
	: ThreadPriority(InPriority), PrimeQueue(InPrimeQueue), PrimeCounter(0) {
	UE_LOG(LogTemp, Warning, TEXT("RunnableGoon Constructor called!"));
}

RunnableGoon::~RunnableGoon() {
	UE_LOG(LogTemp, Warning, TEXT("RunnableGoon Destructor called!"));
}

bool RunnableGoon::Init() {
	UE_LOG(LogTemp, Warning, TEXT("RunnableGoon Init called!"));
	return true;
}

uint32 RunnableGoon::Run() {
	{
		FScopeLock Lock(&CriticalSection);
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		UE_LOG(LogTemp, Warning, TEXT("Thread started with priority: %d, Thread ID: %u"), static_cast<int32>(ThreadPriority), ThreadId);
	}

	while (StopTaskCounter.GetValue() == 0) {
		FindPrimes();
		FPlatformProcess::Sleep(0.1f); // Increased sleep interval to reduce logging frequency
	}

	{
		FScopeLock Lock(&CriticalSection);
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		UE_LOG(LogTemp, Warning, TEXT("Thread exiting Run method! Thread ID: %u"), ThreadId);
	}

	return 0;
}

void RunnableGoon::Stop() {
	StopTaskCounter.Increment();
	{
		FScopeLock Lock(&CriticalSection);
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		UE_LOG(LogTemp, Warning, TEXT("Stop method called! Thread ID: %u"), ThreadId);
	}
}

bool RunnableGoon::IsPrime(int32 Number) {
	if (Number <= 1) return false;
	if (Number <= 3) return true;

	if (Number % 2 == 0 || Number % 3 == 0) return false;

	for (int32 i = 5; i * i <= Number; i += 6) {
		if (Number % i == 0 || Number % (i + 2) == 0) return false;
	}
	return true;
}

void RunnableGoon::FindPrimes() {
	int32 PrimesFound = 0;
	for (int32 i = 2; i < 10000; ++i) {
		if (StopTaskCounter.GetValue() != 0) {
			break;
		}
		if (IsPrime(i)) {
			PrimeCounter++;
			PrimesFound++;
			PrimeQueue->Enqueue(i);
		}
	}

	{
		FScopeLock Lock(&CriticalSection);
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		UE_LOG(LogTemp, Warning, TEXT("Exiting FindPrimes method with %d primes found! Thread ID: %u"), PrimesFound, ThreadId);
	}
}
