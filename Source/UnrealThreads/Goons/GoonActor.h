#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnableGoon.h"
#include "ThreadPoolManager.h"
#include "TaskGraphManager.h"
#include "Particle.h"
#include "../ParticleActor.h"
#include "AddShakeTasksTask.h"
#include "UpdatePositionsTask.h"
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

	TaskGraphManager* TaskGraph;
	ThreadPoolManager* PoolManager;

	TArray<TSharedPtr<FParticle>> Particles;
	TQueue<FParticleUpdate, EQueueMode::Mpsc> PositionUpdateQueue; // Queue for position updates
	FCriticalSection ParticlesCriticalSection; // Critical section for thread safety

	float ElapsedTime = 0.0f; // Timer to track elapsed time

	void InitializeParticles();
	void ScheduleShakeTasks();
	void ScheduleUpdatePositions();


	UPROPERTY(EditAnywhere, Category = "TaskGraph")
	TSubclassOf<AParticleActor> ParticleBP;

	UPROPERTY(EditAnywhere, Category = "TaskGraph")
	int32 ParticleCount = 5000; // Number of particles to simulate

	UPROPERTY(EditAnywhere, Category = "TaskGraph")
	float TaskInterval = 10.f; // Interval in seconds to add new tasks
};
