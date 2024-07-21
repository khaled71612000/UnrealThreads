#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnableGoon.h"
#include "ThreadPoolManager.h"
#include "TaskGraphManager.h"
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
	TaskGraphManager* TaskGraph;

	TQueue<int32, EQueueMode::Mpsc> PrimeQueue; // Thread-safe queue for prime numbers
	FCriticalSection QueueCriticalSection; // Critical section for the prime queue

	int32 TickCounter; // Counter to track the number of ticks
	const int32 TaskAddInterval = 180; // Interval for adding new tasks (e.g., every 60 ticks)
};