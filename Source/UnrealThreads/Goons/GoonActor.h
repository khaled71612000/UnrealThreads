#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnableGoon.h"
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
	TArray<FRunnableThread*> Threads;
	TArray<RunnableGoon*> RunnableGoons;
	//Use Shared Pointers: Use TSharedPtr for managing the promise to ensure it stays valid while it is needed.
	TSharedPtr<TPromise<int32>> PrimePromise;
	TFuture<int32> PrimeFuture;
};


//LogTemp: Warning: Thread started with priority : 0
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//PIE : Server logged in
//PIE : Play in editor total start time 0.247 seconds.
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Exiting FindPrimes method with 1229 primes found!
//LogSlate : Updating window title bar state : overlay mode, drag disabled, window buttons hidden, title bar hidden
//LogWorld : BeginTearingDown for / Game / UEDPIE_0_Untitled
//LogTemp : Warning: Stop method called!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Total number of primes found : 22122
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : RunnableGoon Destructor called!
//LogTemp : Warning : Total number of primes found : 22122