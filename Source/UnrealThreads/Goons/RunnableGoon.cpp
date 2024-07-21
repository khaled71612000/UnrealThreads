#include "RunnableGoon.h"
#include "Misc/OutputDeviceDebug.h"
#include "Windows/WindowsHWrapper.h"

RunnableGoon::RunnableGoon(EThreadPriority InPriority, TQueue<TSharedPtr<ITask>, EQueueMode::Mpsc>& InTaskQueue, FCriticalSection& InTaskQueueCriticalSection, int32 InCoreAffinity)
	: ThreadPriority(InPriority), TaskQueue(InTaskQueue), TaskQueueCriticalSection(InTaskQueueCriticalSection), CoreAffinity(InCoreAffinity) {}

RunnableGoon::~RunnableGoon()
{
}

bool RunnableGoon::Init()
{
	return true;
}

uint32 RunnableGoon::Run()
{
	HANDLE ThreadHandle = GetCurrentThread();
	SetThreadAffinityMask(ThreadHandle, static_cast<DWORD_PTR>(1ULL << CoreAffinity));

	while (StopTaskCounter.GetValue() == 0)
	{
		ExecuteTasks();
		FPlatformProcess::Sleep(0.05f);
	}


	return 0;
}

void RunnableGoon::Stop()
{
	StopTaskCounter.Increment();
}

void RunnableGoon::AddTask(TSharedPtr<ITask> Task)
{
	FScopeLock Lock(&TaskQueueCriticalSection);
	TaskQueue.Enqueue(Task);
}

void RunnableGoon::ExecuteTasks()
{
	FScopeLock Lock(&TaskQueueCriticalSection);
	TSharedPtr<ITask> Task;
	while (TaskQueue.Dequeue(Task))
	{
		if (Task.IsValid())
		{
			Task->Execute();
		}
	}
}
