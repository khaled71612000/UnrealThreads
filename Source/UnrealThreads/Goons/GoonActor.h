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

	TQueue<int32, EQueueMode::Mpsc> PrimeQueue; // Thread-safe queue
	FCriticalSection QueueCriticalSection; // Critical section for the queue
};
