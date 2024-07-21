#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"
#include "Windows/WindowsHWrapper.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority, TQueue<TSharedPtr<ITask>, EQueueMode::Mpsc>& InTaskQueue, FCriticalSection& InTaskQueueCriticalSection, int32 InCoreAffinity)
	: ThreadPriority(InPriority), TaskQueue(InTaskQueue), TaskQueueCriticalSection(InTaskQueueCriticalSection), CoreAffinity(InCoreAffinity) {}

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
	HANDLE ThreadHandle = GetCurrentThread();
	SetThreadAffinityMask(ThreadHandle, static_cast<DWORD_PTR>(1ULL << CoreAffinity));

	{
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Thread started with priority: %d on core: %d"), static_cast<int32>(ThreadPriority), CoreAffinity);
	}
	while (StopTaskCounter.GetValue() == 0)
	{
		ExecuteTasks();
		FPlatformProcess::Sleep(0.05f);
	}

	{
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Thread exiting Run method!"));
	}

	return 0;
}

void RunnableGoon::Stop()
{
	StopTaskCounter.Increment();
	{
		FScopeLock Lock(&CriticalSection);
		UE_LOG(LogTemp, Warning, TEXT("Stop method called!"));
	}
}

void RunnableGoon::AddTask(TSharedPtr<ITask> Task)
{
	FScopeLock Lock(&TaskQueueCriticalSection);
	TaskQueue.Enqueue(Task);
	UE_LOG(LogTemp, Warning, TEXT("[%s] Task added to RunnableGoon queue"), *FDateTime::Now().ToString());
}

void RunnableGoon::ExecuteTasks()
{

	TSharedPtr<ITask> Task;
	while (TaskQueue.Dequeue(Task))
	{
		if (Task.IsValid())
		{
			FScopeLock Lock(&TaskQueueCriticalSection);
			Task->Execute();
		}
	}
}
