#include "MyPhysicsActor.h"
#include "Async/Async.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMyPhysicsActor::AMyPhysicsActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the properties
	CubeMesh = nullptr;
	CubeMaterial = nullptr;
	NumCubes = 10; // Default number of cubes
	CubeSpacing = 200.0f; // Default spacing between cubes
}

// Called when the game starts or when spawned
void AMyPhysicsActor::BeginPlay()
{
	Super::BeginPlay();

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
	Async(EAsyncExecution::Thread, [this]()
		{
			TArray<FVector> Positions;

			// Calculate positions for the cubes
			for (int32 i = 0; i < NumCubes; ++i)
			{
				Positions.Add(FVector(i * CubeSpacing, 0.0f, 300.0f));
			}

			// Schedule the cube spawning on the game thread
			AsyncTask(ENamedThreads::GameThread, [this, Positions]()
				{
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
				});
		});
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