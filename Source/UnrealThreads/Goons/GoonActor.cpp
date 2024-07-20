#include "GoonActor.h"

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

	// Initialize promise and future
	PrimePromise = MakeShared<TPromise<int32>>();
	PrimeFuture = PrimePromise->GetFuture();

	RunnableGoons.Add(new RunnableGoon(TPri_Normal, PrimePromise));

	// Start threads with different priorities
	Threads.Add(FRunnableThread::Create(RunnableGoons[0], TEXT("NormalPriorityThread"), 0, TPri_Normal));
}

// Called every frame
void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	// Check if the future has been fulfilled
	if (PrimeFuture.IsReady())
	{
		int32 PrimeCount = PrimeFuture.Get();
		UE_LOG(LogTemp, Warning, TEXT("Total number of primes found: %d"), PrimeCount);
	}

	Super::EndPlay(EndPlayReason);
}
