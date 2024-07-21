#pragma once

#include "CoreMinimal.h"
#include "TaskNode.h"
#include "ThreadPoolManager.h"

class TaskGraphManager
{
public:
	TaskGraphManager(ThreadPoolManager* InPoolManager)
		: PoolManager(InPoolManager) {}

	void AddTask(TSharedPtr<FTaskNode> Node)
	{
		TaskNodes.Add(Node);
		Node->OnReadyToExecute.AddLambda([this, Node]()
			{
				EnqueueTask(Node);
			});
	}

	void EnqueueTask(TSharedPtr<FTaskNode> Node)
	{
		//In AddTask, an event is bound to the OnReadyToExecute event of the node, which will enqueue the task when the node is ready.
		PoolManager->AddTask(Node->GetTask());
	}

	void Execute()
	{
		for (auto& Node : TaskNodes)
		{
			if (Node->AreDependenciesCompleted())
			{
				Node->MarkReady();
			}
		}
	}

	void MarkTaskCompleted(TSharedPtr<FTaskNode> Node)
	{
		Node->MarkCompleted();
	}

private:
	TArray<TSharedPtr<FTaskNode>> TaskNodes;
	ThreadPoolManager* PoolManager;
};
