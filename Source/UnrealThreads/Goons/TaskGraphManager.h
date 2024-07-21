#pragma once

#include "CoreMinimal.h"
#include "TaskNode.h"
#include "ThreadPoolManager.h"

class TaskGraphManager
{
public:
	TaskGraphManager(ThreadPoolManager* InPoolManager)
		: PoolManager(InPoolManager), bIsExecuting(false) {}

	void AddTask(TSharedPtr<FTaskNode> Node)
	{
		FScopeLock Lock(&TaskCriticalSection); // Lock the critical section
		NewTaskNodes.Add(Node);
		Node->OnReadyToExecute.AddLambda([this, Node]()
			{
				EnqueueTask(Node);
			});
	}

	void Execute()
	{
		{
			FScopeLock Lock(&TaskCriticalSection); // Lock the critical section
			if (bIsExecuting) return; // If already executing, exit
			bIsExecuting = true;

			// Move new tasks to executing tasks
			ExecutingTaskNodes.Append(NewTaskNodes);
			NewTaskNodes.Empty();
		}

		for (auto& Node : ExecutingTaskNodes)
		{
			if (Node->AreDependenciesCompleted())
			{
				Node->MarkReady();
			}
		}

		{
			FScopeLock Lock(&TaskCriticalSection); // Lock the critical section
			bIsExecuting = false;
		}
	}

	bool HasPendingTasks() const
	{
		FScopeLock Lock(&TaskCriticalSection); // Lock the critical section
		return !ExecutingTaskNodes.IsEmpty() || !NewTaskNodes.IsEmpty();
	}

private:
	TArray<TSharedPtr<FTaskNode>> ExecutingTaskNodes;
	TArray<TSharedPtr<FTaskNode>> NewTaskNodes;
	ThreadPoolManager* PoolManager;
	mutable FCriticalSection TaskCriticalSection; // Critical section for thread safety
	bool bIsExecuting; // Flag to indicate if tasks are being executed

	void EnqueueTask(TSharedPtr<FTaskNode> Node)
	{
		PoolManager->AddTask(Node->GetTask());
	}
};
