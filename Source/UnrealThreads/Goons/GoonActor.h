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
	// Sets default values for this actor's properties
	AGoonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/*FRunnableThread is a class in Unreal Engine that manages the lifecycle of a thread running a FRunnable object.It provides functionality to create,
		run, and terminate threads.The FRunnableThread class handles the lower - level details of thread management, allowing you to focus on the work you want to perform in the thread.*/
	TArray<FRunnableThread*> Threads;

	TArray<RunnableGoon*> RunnableGoons;
};
