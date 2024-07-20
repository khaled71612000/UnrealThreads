#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnableGoon.h"
#include "Containers/Queue.h"
#include "GoonActor.generated.h"

UCLASS()
class UNREALTHREADS_API AGoonActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	TArray<FRunnableThread*> Threads;
	TArray<RunnableGoon*> RunnableGoons;
	TQueue<int32, EQueueMode::Mpsc> PrimeQueue; // Shared queue for primes

private:
	float TimeSinceLastDeque = 0.0f; // Timer to control dequeuing frequency
	const float DequeueInterval = 1.0f; // Time interval between dequeues in seconds
	const int32 MaxDequeuesPerTick = 5; // Max number of items to dequeue per tick
};
