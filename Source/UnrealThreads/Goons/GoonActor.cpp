#include "GoonActor.h"
#include "PrimeCalculationTask.h"
#include "ParticleShakeTask.h"
#include "TaskGraphManager.h"
#include "TaskNode.h"
#include "Containers/Array.h"
#include "Templates/SharedPointer.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AGoonActor::AGoonActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PoolManager = nullptr;
	TaskGraph = nullptr;
}

void AGoonActor::BeginPlay()
{
	Super::BeginPlay();
	PoolManager = new ThreadPoolManager(15);
	TaskGraph = new TaskGraphManager(PoolManager);

	InitializeParticles();

	TaskGraph->Execute();
}

void AGoonActor::InitializeParticles()
{
	for (int32 i = 0; i < ParticleCount; ++i)
	{
		// Increase the spawn radius
		FVector InitialPosition = FVector(FMath::FRandRange(-5000.0f, 5000.0f), FMath::FRandRange(-5000.0f, 5000.0f), FMath::FRandRange(0, 5000.0f));
		AParticleActor* ParticleActor = GetWorld()->SpawnActor<AParticleActor>(AParticleActor::StaticClass(), InitialPosition, FRotator::ZeroRotator);
		TSharedPtr<FParticle> Particle = MakeShareable(new FParticle(InitialPosition, ParticleActor, PositionUpdateQueue));
		Particles.Add(Particle);

		TSharedPtr<ITask> ShakeTask = MakeShareable(new ParticleShakeTask(Particle));
		TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(ShakeTask));
		TaskGraph->AddTask(TaskNode);
	}
}

void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update elapsed time
	ElapsedTime += DeltaTime;

	// Check if it's time to add new tasks
	if (ElapsedTime >= TaskInterval)
	{
		AddShakeTasks();
		ElapsedTime = 0.0f; // Reset the timer

		TaskGraph->Execute();
	}

	// Process position updates on the game thread
	UpdateParticlePositions();

	// Additional behavior for each tick if needed
}

void AGoonActor::AddShakeTasks()
{
	for (TSharedPtr<FParticle> Particle : Particles)
	{
		TSharedPtr<ITask> ShakeTask = MakeShareable(new ParticleShakeTask(Particle));
		TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(ShakeTask));
		TaskGraph->AddTask(TaskNode);
	}
}

void AGoonActor::UpdateParticlePositions()
{
	FParticleUpdate Update;
	while (PositionUpdateQueue.Dequeue(Update))
	{
		if (Update.Actor)
		{
			Update.Actor->SetActorLocation(Update.NewPosition);
		}
	}
}

void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PoolManager)
	{
		PoolManager->StopAll(); // Ensure all threads are stopped before cleanup
		delete PoolManager;
		PoolManager = nullptr;
	}

	if (TaskGraph)
	{
		delete TaskGraph;
		TaskGraph = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}
