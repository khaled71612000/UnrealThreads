#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnableGoon.h"
#include "ThreadPoolManager.h"
#include "GoonActor.generated.h"

UCLASS()
class UNREALTHREADS_API AGoonActor : public AActor
{
	GENERATED_BODY()

public:
	AGoonActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	ThreadPoolManager* PoolManager;

	TQueue<int32, EQueueMode::Mpsc> PrimeQueue; // Thread-safe queue for prime numbers
	FCriticalSection QueueCriticalSection; // Critical section for the prime queue

	int32 TickCounter; // Counter to track the number of ticks
	const int32 TaskAddInterval = 180; // Interval for adding new tasks (e.g., every 60 ticks)
};

//System Workflow
//Initialization(AGoonActor::BeginPlay) :
//
//	AGoonActor initializes the ThreadPoolManager with a specified pool size.
//	Submits tasks(instances of PrimeCalculationTask) to the ThreadPoolManager.
//	Each PrimeCalculationTask is created with a number to check for primality and references to PrimeQueue and QueueCriticalSection.
//	Task Submission(ThreadPoolManager::SubmitTask) :
//
//	ThreadPoolManager enqueues the submitted tasks into TaskQueue.
//	RunnableGoon threads fetch tasks from TaskQueue.
//	Task Execution(RunnableGoon::Run and RunnableGoon::ExecuteTasks) :
//
//	Each RunnableGoon thread continuously fetches tasks from TaskQueue.
//	Calls the Execute method of each task.
//	For PrimeCalculationTask, the Execute method checks if the number is prime and, if so, enqueues it into PrimeQueue.
//	Prime Number Logging(AGoonActor::Tick) :
//
//	AGoonActor periodically checks PrimeQueue for new prime numbers.
//	Logs the prime numbers.
//	Cleanup(AGoonActor::EndPlay) :
//
//	AGoonActor calls StopAll on the ThreadPoolManager to signal all threads to stop.
//	Waits for all threads to complete.
//	Deletes the ThreadPoolManager and cleans up resources.
//	Summary
//	ITask : Defines a common interface for tasks.
//	PrimeCalculationTask : Implements the task of checking for prime numbers.
//	RunnableGoon : Worker threads that execute tasks.
//	ThreadPoolManager : Manages the pool of worker threads and distributes tasks.
//	AGoonActor : Initializes the thread pool, submits tasks, processes results, and cleans up resources.
//	This system ensures efficient multithreading, allowing tasks to be processed concurrently and results to be managed in a thread - safe manner.

//LogTemp: Warning: Thread started with priority : 0 on core : 0
//LogTemp : Warning : Thread started with priority : 0 on core : 1
//LogTemp : Warning : Thread started with priority : 0 on core : 2
//PIE : Server logged in
//PIE : Play in editor total start time 0.286 seconds.
//LogTemp : Warning : Prime number found : 2
//LogTemp : Warning : Prime number found : 3
//LogTemp : Warning : Prime number found : 5
//LogTemp : Warning : Prime number found : 7
//LogTemp : Warning : Prime number found : 11
//LogTemp : Warning : Prime number found : 13
//LogTemp : Warning : Prime number found : 17
//LogTemp : Warning : Prime number found : 19
//LogTemp : Warning : Prime number found : 23
//LogTemp : Warning : Prime number found : 29
//LogTemp : Warning : Prime number found : 31
//LogTemp : Warning : Prime number found : 37
//LogTemp : Warning : Prime number found : 41
//LogTemp : Warning : Prime number found : 43
//LogTemp : Warning : Prime number found : 47
//LogTemp : Warning : Prime number found : 53
//LogTemp : Warning : Prime number found : 59
//LogTemp : Warning : Prime number found : 61
//LogTemp : Warning : Prime number found : 67
//LogTemp : Warning : Prime number found : 71
//LogTemp : Warning : Prime number found : 73
//LogTemp : Warning : Prime number found : 79
//LogTemp : Warning : Prime number found : 83
//LogTemp : Warning : Prime number found : 89
//LogTemp : Warning : Prime number found : 97
//LogSlate : Updating window title bar state : overlay mode, drag disabled, window buttons hidden, title bar hidden
//LogWorld : BeginTearingDown for / Game / UEDPIE_0_Untitled
//LogTemp : Warning: Stop method called!
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Stop method called!