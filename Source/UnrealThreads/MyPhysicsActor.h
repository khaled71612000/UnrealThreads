#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "../../../../../../../Source/Runtime/PhysicsCore/Public/PhysicalMaterials/PhysicalMaterial.h"
#include "MyPhysicsActor.generated.h"

UCLASS()
class UNREALTHREADS_API AMyPhysicsActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyPhysicsActor();

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

};
