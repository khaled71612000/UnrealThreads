#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnableGoon.h"
#include "ThreadPoolManager.h"
#include "TaskGraphManager.h"
#include "Particle.h"
#include "../ParticleActor.h"
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

	TArray<TSharedPtr<FParticle>> Particles;
	TQueue<FParticleUpdate, EQueueMode::Mpsc> PositionUpdateQueue; // Queue for position updates

	const int32 ParticleCount = 30000; // Number of particles to simulate
	const float TaskInterval = 2.5f; // Interval in seconds to add new tasks
	float ElapsedTime = 0.0f; // Timer to track elapsed time

	void InitializeParticles();
	void UpdateParticlePositions();
	void AddShakeTasks();
};
