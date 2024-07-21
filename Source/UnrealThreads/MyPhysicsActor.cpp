#include "MyPhysicsActor.h"
#include "Async/Async.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMyPhysicsActor::AMyPhysicsActor()/*:
PositionCalculationCompletedEvent(FPlatformProcess::GetSynchEventFromPool(true)),
AnotherBackgroundTaskCompletedEvent(FPlatformProcess::GetSynchEventFromPool(true))*/
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the properties
	CubeMesh = nullptr;
	CubeMaterial = nullptr;
	NumCubes = 10; // Default number of cubes
	CubeSpacing = 200.0f; // Default spacing between cubes

	// Bind event handlers
	OnPositionCalculationCompleted.AddUObject(this, &AMyPhysicsActor::CheckAllTasksCompleted);
	OnAnotherBackgroundTaskCompleted.AddUObject(this, &AMyPhysicsActor::CheckAllTasksCompleted);

	CompletedTasks = 0;
}
// Called when the game starts or when spawned
void AMyPhysicsActor::BeginPlay()
{
	Super::BeginPlay();
	CompletedTasks = 0;
	// Spawn the physics cubes
	SpawnPhysicsCubes();
}

// Called every frame
void AMyPhysicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function to spawn physics cubes
void AMyPhysicsActor::SpawnPhysicsCubes()
{
	// Number of tasks to wait for
	const int32 NumTasks = 2;

	Async(EAsyncExecution::Thread, [this]()
		{
			TArray<FVector> Positions;
			//Setting the size of the array (Positions.SetNum(NumCubes);) is crucial because it pre-allocates the required memory for the array before it is accessed in parallel. This ensures that each index in the array is valid and prevents out-of-bounds access.
			//ParallelFor divides the loop iterations across multiple threads, and each thread operates on different indices of the array.
			//Pre-allocating the array ensures that all threads have valid indices to work with, preventing any concurrent modification issues or invalid memory accesses.
			Positions.SetNum(NumCubes);  // Ensure Positions array is correctly sized

			// Calculate positions for the cubes in a sequencal matter then move into async task which will take the rest of the code to the game therad
			/*for (int32 i = 0; i < NumCubes; ++i)
			{
				Positions.Add(FVector(i * CubeSpacing, 0.0f, 300.0f));
			}*/
			ParallelFor(NumCubes, [this, &Positions](int32 Index)
				{
					Positions[Index] = FVector(Index * CubeSpacing, 0.0f, 300.0f);
				});

			// Schedule the cube spawning on the game thread
			OnPositionCalculationCompleted.Broadcast();

		});

	// Task 2: Another background operation (e.g., loading data)
	Async(EAsyncExecution::Thread, [this, NumTasks]()
		{
			// Simulate another background task
			FPlatformProcess::Sleep(1.0f); // Simulate work

			// Signal task completion
			OnAnotherBackgroundTaskCompleted.Broadcast();
		});

	//// Wait for all background tasks to complete
	//Async(EAsyncExecution::Thread, [this, NumTasks]()
	//	{
	//		PositionCalculationCompletedEvent->Wait();
	//		AnotherBackgroundTaskCompletedEvent->Wait();

	//		// Call the function to handle the completion on the game thread
	//		AsyncTask(ENamedThreads::GameThread, [this]()
	//			{
	//				OnBackgroundTasksCompleted();
	//			});
	//	});
}

// Internal helper to check if all tasks are completed
void AMyPhysicsActor::CheckAllTasksCompleted()
{
	CompletedTasks++;

	if (CompletedTasks == 2) // Number of tasks to wait for
	{
		// Call the function to handle the completion on the game thread
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				OnBackgroundTasksCompleted();
			});
	}
}

// Function to apply random forces to cubes
void AMyPhysicsActor::ApplyRandomForces()
{
	for (UStaticMeshComponent* Cube : CubeComponents)
	{
		if (Cube && Cube->IsSimulatingPhysics())
		{
			FVector RandomForce = FVector(FMath::RandRange(-20000.0f, 20000.0f), 0, 0);
			Cube->AddForce(RandomForce);
		}
	}

	// Schedule the next application of random forces
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMyPhysicsActor::ApplyRandomForces);
}

void AMyPhysicsActor::OnBackgroundTasksCompleted()
{
	TArray<FVector> Positions;
	Positions.SetNum(NumCubes);

	for (const FVector& Position : Positions)
	{
		// Create a new static mesh component for each cube
		UStaticMeshComponent* Cube = NewObject<UStaticMeshComponent>(this);

		// Attach the component to the actor
		Cube->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// Set the static mesh and material
		Cube->SetStaticMesh(CubeMesh);
		Cube->SetMaterial(0, CubeMaterial);

		// Set the position
		Cube->SetRelativeLocation(Position);

		// Enable physics
		Cube->SetSimulatePhysics(true);
		Cube->SetMobility(EComponentMobility::Movable);

		// Assign the physical material
		if (CubePhysicalMaterial)
		{
			Cube->SetPhysMaterialOverride(CubePhysicalMaterial);
		}

		// Register the component so it appears in the game
		Cube->RegisterComponent();

		// Add the cube to the array
		CubeComponents.Add(Cube);
	}

	// Start applying random forces
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMyPhysicsActor::ApplyRandomForces);
}