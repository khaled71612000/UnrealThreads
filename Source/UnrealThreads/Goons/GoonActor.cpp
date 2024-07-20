#include "GoonActor.h"

AGoonActor::AGoonActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGoonActor::BeginPlay()
{
	Super::BeginPlay();

	RunnableGoons.Add(new RunnableGoon(TPri_Highest, PrimeQueue, QueueCriticalSection, 0)); // Core 0
	RunnableGoons.Add(new RunnableGoon(TPri_Normal, PrimeQueue, QueueCriticalSection, 1)); // Core 1
	RunnableGoons.Add(new RunnableGoon(TPri_Lowest, PrimeQueue, QueueCriticalSection, 2)); // Core 2

	Threads.Add(FRunnableThread::Create(RunnableGoons[0], TEXT("HighPriorityThread"), 0, TPri_Highest));
	Threads.Add(FRunnableThread::Create(RunnableGoons[1], TEXT("NormalPriorityThread"), 0, TPri_Normal));
	Threads.Add(FRunnableThread::Create(RunnableGoons[2], TEXT("LowPriorityThread"), 0, TPri_Lowest));
}

void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//int32 PrimeNumber;
	//while (PrimeQueue.Dequeue(PrimeNumber))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Prime number found: %d"), PrimeNumber);
	//}
}

void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
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

	Super::EndPlay(EndPlayReason);
}
