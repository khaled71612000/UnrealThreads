#include "GoonActor.h"

// Sets default values
AGoonActor::AGoonActor() {
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGoonActor::BeginPlay() {
	Super::BeginPlay();

	RunnableGoons.Add(new RunnableGoon(TPri_Normal, &PrimeQueue));
	Threads.Add(FRunnableThread::Create(RunnableGoons[0], TEXT("NormalPriorityThread"), 0, TPri_Normal));
}

// Called every frame
void AGoonActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	TimeSinceLastDeque += DeltaTime;
	if (TimeSinceLastDeque >= DequeueInterval) {
		TimeSinceLastDeque = 0.0f;
		int32 Prime;
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		for (int32 i = 0; i < MaxDequeuesPerTick; ++i) {
			if (PrimeQueue.Dequeue(Prime)) {
				UE_LOG(LogTemp, Warning, TEXT("Prime number: %d, Thread ID: %u"), Prime, ThreadId);
			}
			else {
				break; // No more items to dequeue
			}
		}
	}
}


// Called when the game ends or the actor is destroyed
void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (RunnableGoon* Runnable : RunnableGoons) {
		if (Runnable) {
			Runnable->Stop();
		}
	}

	for (FRunnableThread* Thread : Threads) {
		if (Thread) {
			Thread->WaitForCompletion();
			delete Thread;
			Thread = nullptr;
		}
	}

	for (RunnableGoon* Runnable : RunnableGoons) {
		if (Runnable) {
			delete Runnable;
			Runnable = nullptr;
		}
	}

	Super::EndPlay(EndPlayReason);
}
