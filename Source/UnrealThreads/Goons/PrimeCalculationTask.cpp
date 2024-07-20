#include "PrimeCalculationTask.h"

PrimeCalculationTask::PrimeCalculationTask(int32 InNumber, TQueue<int32, EQueueMode::Mpsc>& InQueue, FCriticalSection& InQueueCriticalSection)
	: Number(InNumber), Queue(InQueue), QueueCriticalSection(InQueueCriticalSection) {}

void PrimeCalculationTask::Execute()
{
	UE_LOG(LogTemp, Warning, TEXT("PrimeCalculationTask executing for number: %d"), Number);
	if (IsPrime(Number))
	{
		FScopeLock Lock(&QueueCriticalSection);
		Queue.Enqueue(Number);
		UE_LOG(LogTemp, Warning, TEXT("Prime number %d found and enqueued"), Number);
	}
}

bool PrimeCalculationTask::IsPrime(int32 Num)
{
	if (Num <= 1) return false;
	if (Num <= 3) return true;

	if (Num % 2 == 0 || Num % 3 == 0) return false;

	for (int32 i = 5; i * i <= Num; i += 6)
	{
		if (Num % i == 0 || Num % (i + 2) == 0)
			return false;
	}
	return true;
}
