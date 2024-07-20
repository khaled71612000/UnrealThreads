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

//LogTemp: Warning: RunnableGoon Constructor called!
//LogTemp : Warning : RunnableGoon Init called!
//LogTemp : Warning : Thread started with priority : 0, Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//PIE : Server logged in
//PIE : Play in editor total start time 0.285 seconds.
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Prime number : 2, Thread ID : 32528
//LogTemp : Warning : Prime number : 3, Thread ID : 32528
//LogTemp : Warning : Prime number : 5, Thread ID : 32528
//LogTemp : Warning : Prime number : 7, Thread ID : 32528
//LogTemp : Warning : Prime number : 11, Thread ID : 32528
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Prime number : 13, Thread ID : 32528
//LogTemp : Warning : Prime number : 17, Thread ID : 32528
//LogTemp : Warning : Prime number : 19, Thread ID : 32528
//LogTemp : Warning : Prime number : 23, Thread ID : 32528
//LogTemp : Warning : Prime number : 29, Thread ID : 32528
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Prime number : 31, Thread ID : 32528
//LogTemp : Warning : Prime number : 37, Thread ID : 32528
//LogTemp : Warning : Prime number : 41, Thread ID : 32528
//LogTemp : Warning : Prime number : 43, Thread ID : 32528
//LogTemp : Warning : Prime number : 47, Thread ID : 32528
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Prime number : 53, Thread ID : 32528
//LogTemp : Warning : Prime number : 59, Thread ID : 32528
//LogTemp : Warning : Prime number : 61, Thread ID : 32528
//LogTemp : Warning : Prime number : 67, Thread ID : 32528
//LogTemp : Warning : Prime number : 71, Thread ID : 32528
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Prime number : 73, Thread ID : 32528
//LogTemp : Warning : Prime number : 79, Thread ID : 32528
//LogTemp : Warning : Prime number : 83, Thread ID : 32528
//LogTemp : Warning : Prime number : 89, Thread ID : 32528
//LogTemp : Warning : Prime number : 97, Thread ID : 32528
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!Thread ID : 35280
//LogSlate : Updating window title bar state : overlay mode, drag disabled, window buttons hidden, title bar hidden
//LogWorld : BeginTearingDown for / Game / UEDPIE_0_Untitled
//LogTemp : Warning: Stop method called!Thread ID : 32528
//LogTemp : Warning : Thread exiting Run method!Thread ID : 35280
//LogTemp : Warning : Stop method called!Thread ID : 32528
//LogTemp : Warning : RunnableGoon Destructor called!