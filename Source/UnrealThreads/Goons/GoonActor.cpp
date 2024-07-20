#include "GoonActor.h"
#include "../../../../../../../Source/Runtime/Core/Public/Windows/WindowsSemaphore.h"

// Sets default values
AGoonActor::AGoonActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGoonActor::BeginPlay()
{
	Super::BeginPlay();

	// Create a semaphore with an initial count of 0 and maximum count of 1
	Semaphore = new FWindowsSemaphore(1, 2);

	/*Imagine you have multiple threads that need to access a shared resource, such as a file, database,
		or in this case, a list of prime numbers.To prevent these threads from interfering with each other and causing data corruption,
		you use a semaphore to control access.*/

	// Create runnables with different priorities and pass the semaphore
	RunnableGoons.Add(new RunnableGoon(TPri_Highest, Semaphore));
	RunnableGoons.Add(new RunnableGoon(TPri_Normal, Semaphore));
	RunnableGoons.Add(new RunnableGoon(TPri_Lowest, Semaphore));

	// Start threads with different priorities
	Threads.Add(FRunnableThread::Create(RunnableGoons[0], TEXT("HighPriorityThread"), 0, TPri_Highest));
	Threads.Add(FRunnableThread::Create(RunnableGoons[1], TEXT("NormalPriorityThread"), 0, TPri_Normal));
	Threads.Add(FRunnableThread::Create(RunnableGoons[2], TEXT("LowPriorityThread"), 0, TPri_Lowest));
}
// Called every frame
void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("EndPlay called with reason: %d"), static_cast<int32>(EndPlayReason));

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
			delete Thread;
			Thread = nullptr;
		}
	}

	for (RunnableGoon* Runnable : RunnableGoons)
	{
		if (Runnable)
		{
			delete Runnable;
			Runnable = nullptr;
		}
	}

	// Delete the semaphore
	if (Semaphore)
	{
		delete Semaphore;
		Semaphore = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}
