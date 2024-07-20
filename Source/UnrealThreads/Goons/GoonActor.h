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


	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FWindowsSemaphore* Semaphore;  // Declare as a pointer for proper cleanup

	TArray<FRunnableThread*> Threads;
	TArray<RunnableGoon*> RunnableGoons;
};
