#include "GoonActor.h"
#include "PrimeCalculationTask.h"

AGoonActor::AGoonActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PoolManager = nullptr;
}

void AGoonActor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the thread pool manager with a pool size of 5
	//If you pass a pool size of 100 to the ThreadPoolManager, several things can happen depending on your system's hardware and the design of the thread pool manager. Here are the key considerations:
	//Task Distribution Timing:

	//When tasks are enqueued, if Thread 4 happens to dequeue them faster, it might start processing more tasks before other threads get a chance.
	//	Thread Scheduling :

	//The operating system's thread scheduler might assign more CPU time to Thread 4 initially, causing it to process more tasks.
	//	Batch Submission :

	//If tasks are enqueued in batches and threads are picking them up from the queue, the first thread to start processing might take more tasks from the queue.
	PoolManager = new ThreadPoolManager(5);


	// Create an array of tasks
	// add to a tarray a shareable pointer of created pointer of the constructor
	TArray<TSharedPtr<ITask>> Tasks;
	for (int32 i = 2; i < 100; ++i)
	{
		Tasks.Add(MakeShareable(new PrimeCalculationTask(i, PrimeQueue, QueueCriticalSection)));
	}

	// Submit the array of tasks
	PoolManager->SubmitTasks(Tasks);
}

void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Calling Tick every frame could potentially introduce contention and locking issues, thats why we dont put a critcal section here
	// especially if multiple threads frequently access the PrimeQueue. In a high-performance game environment, this could lead to performance degradation.
	//so we introduce a local prime buffer to copy data and shorten the time inside the while or the time we take to log is too short anyways
	{
		FScopeLock Lock(&QueueCriticalSection);
		int32 PrimeNumber;
		while (PrimeQueue.Dequeue(PrimeNumber))
		{
			UE_LOG(LogTemp, Warning, TEXT("Prime number found: %d"), PrimeNumber);
		}
	}

	// Increment the tick counter
	TickCounter++;

	// Check if it's time to add more tasks
	if (TickCounter >= TaskAddInterval)
	{
		// Reset the counter
		TickCounter = 0;

		// Add more tasks
		TArray<TSharedPtr<ITask>> NewTasks;
		//Using static for the StartNumber variable in the Tick function ensures that its value is preserved across multiple calls to the function.
		//This allows the function to maintain a consistent state between ticks, ensuring that the range of numbers for new tasks continues from where it left off in the previous interval.
		static int32 StartNumber = 100;
		int32 EndNumber = StartNumber + 50;
		for (int32 i = StartNumber; i < EndNumber; ++i)
		{
			NewTasks.Add(MakeShareable(new PrimeCalculationTask(i, PrimeQueue, QueueCriticalSection)));
		}

		// Submit the new tasks to the pool manager
		PoolManager->SubmitTasks(NewTasks);

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

	Super::EndPlay(EndPlayReason);
}
