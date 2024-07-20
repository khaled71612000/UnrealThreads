#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformAtomics.h"
#include "Containers/Queue.h"
#include "ITask.h"

class UNREALTHREADS_API RunnableGoon : public FRunnable
{
public:
	RunnableGoon(EThreadPriority InPriority, TQueue<TSharedPtr<ITask>, EQueueMode::Mpsc>& InTaskQueue, FCriticalSection& InTaskQueueCriticalSection, int32 CoreAffinity);
	virtual ~RunnableGoon();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	void AddTask(TSharedPtr<ITask> Task);

private:
	FThreadSafeCounter StopTaskCounter;
	EThreadPriority ThreadPriority;
	FCriticalSection CriticalSection;

	TQueue<TSharedPtr<ITask>, EQueueMode::Mpsc>& TaskQueue;
	FCriticalSection& TaskQueueCriticalSection;

	int32 CoreAffinity; // Core affinity for this thread

	void ExecuteTasks();
};
