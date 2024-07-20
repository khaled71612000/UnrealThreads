#include "GoonActor.h"

AGoonActor::AGoonActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PoolManager = nullptr;
}

void AGoonActor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the thread pool manager with a pool size of 3
	PoolManager = new ThreadPoolManager(3, PrimeQueue, QueueCriticalSection);
}

void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 PrimeNumber;
	while (PrimeQueue.Dequeue(PrimeNumber))
	{
		UE_LOG(LogTemp, Warning, TEXT("Prime number found: %d"), PrimeNumber);
	}
}

void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PoolManager)
	{
		PoolManager->StopAll();
		delete PoolManager;
		PoolManager = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}
