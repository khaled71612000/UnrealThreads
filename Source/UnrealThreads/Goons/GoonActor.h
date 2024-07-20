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

	TArray<FRunnableThread*> Threads;

	TArray<RunnableGoon*> RunnableGoons;

    TQueue<int32, EQueueMode::Mpsc> PrimeQueue; // Thread-safe queue
    FCriticalSection QueueCriticalSection; // Critical section for the queue
};


//LogTemp: Warning: Thread started with priority : 3 on core : 0
//LogTemp : Warning : Thread started with priority : 0 on core : 1
//LogTemp : Warning : Thread started with priority : 4 on core : 2
//PIE : Server logged in
//LogTemp : Warning: Running in a separate thread!
//PIE : Play in editor total start time 0.291 seconds.
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Running in a separate thread!
//LogSlate : Updating window title bar state : overlay mode, drag disabled, window buttons hidden, title bar hidden
//LogWorld : BeginTearingDown for / Game / UEDPIE_0_Untitled
//LogTemp : Warning: Stop method called!
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Total number of primes found : 19664
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Total number of primes found : 19664
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : Running in a separate thread!
//LogTemp : Warning : Thread exiting Run method!
//LogTemp : Warning : Total number of primes found : 19664
//LogTemp : Warning : Stop method called!
//LogTemp : Warning : RunnableGoon Destructor called!
//LogTemp : Warning : RunnableGoon Destructor called!
//LogTemp : Warning : RunnableGoon Destructor called!