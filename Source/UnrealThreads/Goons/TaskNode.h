#pragma once

#include "CoreMinimal.h"
#include "ITask.h"

class FTaskNode
{
public:
	DECLARE_EVENT(FTaskNode, FOnReadyToExecute);
	FOnReadyToExecute OnReadyToExecute;

	FTaskNode(TSharedPtr<ITask> InTask)
		: Task(InTask), bIsCompleted(false) {}

	void AddDependency(TSharedPtr<FTaskNode> Dependency)
	{
		Dependencies.Add(Dependency);
	}

	void MarkReady()
	{
		if (AreDependenciesCompleted())
		{
			OnReadyToExecute.Broadcast();
		}
	}

	void MarkCompleted()
	{
		bIsCompleted = true;
		for (auto& DependentNode : Dependents)
		{
			DependentNode->MarkReady();
		}
	}

	bool AreDependenciesCompleted() const
	{
		for (auto& Dependency : Dependencies)
		{
			if (!Dependency->IsCompleted())
			{
				return false;
			}
		}
		return true;
	}

	bool IsCompleted() const
	{
		return bIsCompleted;
	}

	void AddDependent(TSharedPtr<FTaskNode> Dependent)
	{
		Dependents.Add(Dependent);
	}

	TSharedPtr<ITask> GetTask() const
	{
		return Task;
	}

private:
	TSharedPtr<ITask> Task;
	TArray<TSharedPtr<FTaskNode>> Dependencies;
	TArray<TSharedPtr<FTaskNode>> Dependents;
	bool bIsCompleted;
};
