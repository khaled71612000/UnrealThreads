#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "../../../../../../../Source/Runtime/PhysicsCore/Public/PhysicalMaterials/PhysicalMaterial.h"
#include "Async/TaskGraphInterfaces.h"
#include "MyPhysicsActor.generated.h"

UCLASS()
class UNREALTHREADS_API AMyPhysicsActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyPhysicsActor();

	FVector ClampPosition(const FVector& Position, float MaxValue);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Function to spawn physics cubes
	void SpawnPhysicsCubes();

	// Static mesh for the cubes
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* CubeMesh;

	// Material for the cubes
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UMaterialInterface* CubeMaterial;

	// Number of cubes to spawn
	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 NumCubes;

	// Spacing between cubes
	UPROPERTY(EditAnywhere, Category = "Setup")
	float CubeSpacing;

	// Function to apply random forces to cubes
	void ApplyRandomForces();
	TArray<UStaticMeshComponent*> CubeComponents;

	// Physical material for the cubes
	UPROPERTY(EditAnywhere, Category = "Physics")
	UPhysicalMaterial* CubePhysicalMaterial;

	int32 CompletedTasks;

	DECLARE_EVENT(AMyPhysicsActor, FOnTasksCompleted)
	FOnTasksCompleted OnPositionCalculationCompleted;
	FOnTasksCompleted OnAnotherBackgroundTaskCompleted;

	//// Events to signal completion of background tasks
	//FEvent* PositionCalculationCompletedEvent;
	//FEvent* AnotherBackgroundTaskCompletedEvent;

	// Function to handle completion of background tasks
	void OnBackgroundTasksCompleted();

	// Internal helper to check if all tasks are completed
	void CheckAllTasksCompleted();

};

class FMyPhysicsTask : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FMyPhysicsTask>;

public:
	FMyPhysicsTask(int32 InNumCubes, float InCubeSpacing)
		: NumCubes(InNumCubes), CubeSpacing(InCubeSpacing) {}

protected:
	int32 NumCubes;
	float CubeSpacing;

	void DoWork()
	{
		// Perform your background task here
		TArray<FVector> Positions;
		Positions.SetNum(NumCubes);

		ParallelFor(NumCubes, [this, &Positions](int32 Index)
			{
				FVector Position = FVector(Index * CubeSpacing, 0.0f, 300.0f);
				Position = ClampPosition(Position, 1000.0f); // Adjust the max value as needed
				Positions[Index] = Position;
			});
	}

	FVector ClampPosition(const FVector& Position, float MaxValue)
	{
		return FVector(
			FMath::Clamp(Position.X, -MaxValue, MaxValue),
			FMath::Clamp(Position.Y, -MaxValue, MaxValue),
			FMath::Clamp(Position.Z, -MaxValue, MaxValue)
		);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FMyPhysicsTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};