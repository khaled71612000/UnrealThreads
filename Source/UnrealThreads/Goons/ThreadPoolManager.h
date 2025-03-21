#pragma once

#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"
#include "RunnableGoon.h"
#include "Containers/Queue.h"
#include "ITask.h"

class UNREALTHREADS_API ThreadPoolManager
{
public:
	ThreadPoolManager(int32 PoolSize);
	~ThreadPoolManager();

	void StopAll();
	void AddThreads(int32 AdditionalThreads);

	void AddTask(TSharedPtr<ITask> Task)
	{
		int32 Index = FMath::RandHelper(RunnableGoons.Num());
		RunnableGoons[Index]->AddTask(Task);
	}

private:
	TArray<FRunnableThread*> Threads;
	TArray<RunnableGoon*> RunnableGoons;
	int32 PoolSize;

	TQueue<TSharedPtr<ITask>, EQueueMode::Mpsc> TaskQueue; // Reference to the task queue
	FCriticalSection TaskQueueCriticalSection; // Reference to the critical section for task queue

	void CreateThreads(int32 NumThreads);

private:
	int32 LastThreadIndex; // Keeps track of the last thread index used for task distribution
};
