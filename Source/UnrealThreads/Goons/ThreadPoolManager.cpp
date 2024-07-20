#include "ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(int32 InPoolSize, TQueue<int32, EQueueMode::Mpsc>& InQueue, FCriticalSection& InQueueCriticalSection)
	: PoolSize(InPoolSize), Queue(InQueue), QueueCriticalSection(InQueueCriticalSection)
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		RunnableGoons.Add(new RunnableGoon(TPri_Normal, Queue, QueueCriticalSection, i % PoolSize));
		Threads.Add(FRunnableThread::Create(RunnableGoons[i], *FString::Printf(TEXT("ThreadPoolThread%d"), i), 0, TPri_Normal));
	}
}

ThreadPoolManager::~ThreadPoolManager()
{
	StopAll();

	for (RunnableGoon* Runnable : RunnableGoons)
	{
		if (Runnable)
		{
			delete Runnable;
			Runnable = nullptr;
		}
	}

	for (FRunnableThread* Thread : Threads)
	{
		if (Thread)
		{
			delete Thread;
			Thread = nullptr;
		}
	}
}

void ThreadPoolManager::SubmitTask(EThreadPriority InPriority, int32 CoreAffinity)
{
	// Implement task submission logic if needed
}

void ThreadPoolManager::StopAll()
{
	for (RunnableGoon* Runnable : RunnableGoons)
	{
		if (Runnable)
		{
			Runnable->Stop();
		}
	}

	for (FRunnableThread* Thread : Threads)
	{
		if (Thread)
		{
			Thread->WaitForCompletion();
		}
	}
}
