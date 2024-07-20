#pragma once

#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"
#include "RunnableGoon.h"
#include "Containers/Queue.h"

class UNREALTHREADS_API ThreadPoolManager
{
public:
	ThreadPoolManager(int32 PoolSize, TQueue<int32, EQueueMode::Mpsc>& InQueue, FCriticalSection& InQueueCriticalSection);
	~ThreadPoolManager();

	void SubmitTask(EThreadPriority InPriority, int32 CoreAffinity);
	void StopAll();

private:
	TArray<FRunnableThread*> Threads;
	TArray<RunnableGoon*> RunnableGoons;
	int32 PoolSize;

	TQueue<int32, EQueueMode::Mpsc>& Queue; // Reference to the thread-safe queue
	FCriticalSection& QueueCriticalSection; // Reference to the critical section for queue
};
