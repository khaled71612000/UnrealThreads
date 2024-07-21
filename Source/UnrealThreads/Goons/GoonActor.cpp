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
	PoolManager = new ThreadPoolManager(3);
	TaskGraph = new TaskGraphManager(PoolManager);

	InitializeParticles();

	TaskGraph->Execute();
}

void AGoonActor::InitializeParticles()
{
	TArray<TSharedPtr<FParticle>> LocalParticles;
	LocalParticles.Reserve(ParticleCount);

	for (int32 i = 0; i < ParticleCount; ++i)
	{
		FVector InitialPosition = FVector(FMath::FRandRange(-6500.0f, 6500.0f), FMath::FRandRange(-6500.0f, 6500.0f), FMath::FRandRange(100, 3000.0f));
		AParticleActor* ParticleActor = GetWorld()->SpawnActor<AParticleActor>(ParticleBP, InitialPosition, FRotator::ZeroRotator);
		TSharedPtr<FParticle> Particle = MakeShareable(new FParticle(InitialPosition, ParticleActor, PositionUpdateQueue));
		LocalParticles.Add(Particle);

		TSharedPtr<ITask> ShakeTask = MakeShareable(new ParticleShakeTask(Particle));
		TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(ShakeTask));
		TaskGraph->AddTask(TaskNode);
	}

	{
		FScopeLock Lock(&ParticlesCriticalSection);
		Particles.Append(LocalParticles);
	}
}

void AGoonActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ElapsedTime += DeltaTime;

    if (ElapsedTime >= TaskInterval)
    {
        if (!TaskGraph->HasPendingTasks())
        {
            ScheduleShakeTasks();
        }
        ElapsedTime = 0.0f;
    }

    ScheduleUpdatePositions();
    TaskGraph->Execute();
}

void AGoonActor::ScheduleShakeTasks()
{
	TSharedPtr<ITask> AddTasksTask = MakeShareable(new AddShakeTasksTask(this));
	TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(AddTasksTask));
	TaskGraph->AddTask(TaskNode);
}

void AGoonActor::ScheduleUpdatePositions()
{
	TSharedPtr<ITask> UpdateTask = MakeShareable(new UpdatePositionsTask(this));
	TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(UpdateTask));
	TaskGraph->AddTask(TaskNode);
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
