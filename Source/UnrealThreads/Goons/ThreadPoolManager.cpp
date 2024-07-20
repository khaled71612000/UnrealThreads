#include "ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(int32 InPoolSize)
	: PoolSize(InPoolSize), LastThreadIndex(0)
{
	CreateThreads(PoolSize);
}

ThreadPoolManager::~ThreadPoolManager()
{
	StopAll(); // Ensure all threads are stopped before cleanup
}

void ThreadPoolManager::CreateThreads(int32 NumThreads)
{
	for (int32 i = 0; i < NumThreads; ++i)
	{
		//Even Distribution: Ensures that threads are distributed across different cores in a round-robin fashion. This helps in balancing the load across all available cores.
		//The use of RunnableGoons.Last() in the line is based on the assumption that the last added RunnableGoon instance is the one that should be associated with the new thread. Here's why this approach is used
		RunnableGoons.Add(new RunnableGoon(TPri_Normal, TaskQueue, TaskQueueCriticalSection, i % PoolSize));
		Threads.Add(FRunnableThread::Create(
			RunnableGoons.Last(),
			*FString::Printf(TEXT("ThreadPoolThread%d"), Threads.Num()),
			0,
			TPri_Normal));
		UE_LOG(LogTemp, Warning, TEXT("[%s] ThreadPoolThread%d created with Core Affinity %d"), *FDateTime::Now().ToString(), Threads.Num() - 1, i % PoolSize);
	}
}

void ThreadPoolManager::SubmitTask(TSharedPtr<ITask> Task)
{
	//Even though TQueue is thread-safe in Unreal Engine, using additional locking mechanisms like FScopeLock ensures enhanced safety and consistency, especially when dealing with complex multithreading scenarios
	
	// Distribute tasks evenly using round-robin
	int32 ThreadIndex = LastThreadIndex % PoolSize;
	LastThreadIndex++;
	RunnableGoons[ThreadIndex]->AddTask(Task);

	UE_LOG(LogTemp, Warning, TEXT("[%s] Task submitted to queue"), *FDateTime::Now().ToString());
}

void ThreadPoolManager::SubmitTasks(const TArray<TSharedPtr<ITask>>& Tasks)
{
	FScopeLock Lock(&TaskQueueCriticalSection); // Lock once for the entire batch
	for (const TSharedPtr<ITask>& Task : Tasks)
	{
		// Distribute tasks evenly using round-robin
		int32 ThreadIndex = LastThreadIndex % PoolSize;
		LastThreadIndex++;
		RunnableGoons[ThreadIndex]->AddTask(Task);
		UE_LOG(LogTemp, Warning, TEXT("[%s] Task submitted to queue"), *FDateTime::Now().ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Batch of %d tasks submitted to queue"), *FDateTime::Now().ToString(), Tasks.Num());
}
void ThreadPoolManager::StopAll()
{
	// Signal all runnables to stop
	for (RunnableGoon* Runnable : RunnableGoons)
	{
		if (Runnable)
		{
			Runnable->Stop();
		}
	}

	// Wait for all threads to complete
	for (FRunnableThread* Thread : Threads)
	{
		if (Thread)
		{
			Thread->WaitForCompletion();
		}
	}
}

void ThreadPoolManager::AddThreads(int32 AdditionalThreads)
{
	CreateThreads(AdditionalThreads);
}
