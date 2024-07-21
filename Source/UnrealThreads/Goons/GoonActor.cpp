#include "GoonActor.h"
#include "PrimeCalculationTask.h"
#include "TaskGraphManager.h"
#include "TaskNode.h"
#include "Containers/Array.h"
#include "Templates/SharedPointer.h"

AGoonActor::AGoonActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PoolManager = nullptr;
	TaskGraph = nullptr;
}

void AGoonActor::BeginPlay()
{
	Super::BeginPlay();
	TaskGraph = new TaskGraphManager(new ThreadPoolManager(5));

	// Create and add tasks with dependencies
	TArray<TSharedPtr<FTaskNode>> TaskNodes;
	for (int32 i = 2; i < 100; ++i)
	{
		TSharedPtr<ITask> PrimeTask = MakeShareable(new PrimeCalculationTask(i, PrimeQueue, QueueCriticalSection));
		TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(PrimeTask));
		//So you create a task and wrap it then add it to the task graph which will add it to a list of nodes he has and when its time to be executed (task graph to run)
		//he will added it to the thread pool and unwrap the nodes to assign it to a random runnable in the queuedtasks array when one of them will dequeu and execute it
		TaskGraph->AddTask(TaskNode);
		TaskNodes.Add(TaskNode);
	}

	// Example: Set dependencies (Task i depends on Task i-1)
	for (int32 i = 1; i < TaskNodes.Num(); ++i)
	{
		TaskNodes[i]->AddDependency(TaskNodes[i - 1]);
		TaskNodes[i - 1]->AddDependent(TaskNodes[i]);
	}

	// Execute the task graph by goigng through your nodes and each nodes dependinces and if it has no needed depencnines or all depencins are completetd 
	// (we can mark it completed by marking the node in task graph) which will mark the task by going through dependnices and marking them ready to be executed,
	// depencins are tasks and completed is a bool
	// oncec all nodes of nodes are ready it will boradcast OnReadyToExecute
	// or when we execute it doesnt have dependinces
	TaskGraph->Execute();
}

void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Local buffer to hold prime numbers temporarily
	TArray<int32> LocalPrimeBuffer;

	// Lock the critical section only while accessing the shared PrimeQueue
	{
		FScopeLock Lock(&QueueCriticalSection);
		int32 PrimeNumber;
		while (PrimeQueue.Dequeue(PrimeNumber))
		{
			LocalPrimeBuffer.Add(PrimeNumber);
			UE_LOG(LogTemp, Warning, TEXT("Prime number %d dequeued from PrimeQueue"), PrimeNumber);
		}
	}

	// Process the prime numbers outside the critical section
	for (int32 PrimeNumber : LocalPrimeBuffer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Prime number found: %d"), PrimeNumber);
	}

	// Increment the tick counter
	TickCounter++;

	// Check if it's time to add more tasks
	if (TickCounter >= TaskAddInterval)
	{
		// Reset the counter
		TickCounter = 0;

		// Add more tasks
		TArray<TSharedPtr<FTaskNode>> NewTasks;
		static int32 StartNumber = 100;
		int32 EndNumber = StartNumber + 50;
		for (int32 i = StartNumber; i < EndNumber; ++i)
		{
			TSharedPtr<ITask> PrimeTask = MakeShareable(new PrimeCalculationTask(i, PrimeQueue, QueueCriticalSection));
			TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(PrimeTask));
			TaskGraph->AddTask(TaskNode);
			NewTasks.Add(TaskNode);
		}

		// Execute the new tasks in the task graph
		TaskGraph->Execute();

		// Update the start number for the next batch
		StartNumber = EndNumber;
	}
}

void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PoolManager)
	{
		PoolManager->StopAll(); // Ensure all threads are stopped before cleanup
		delete PoolManager;
		PoolManager = nullptr;
	}

	if (TaskGraph)
	{
		delete TaskGraph;
		TaskGraph = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}
