#include "ParticleActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AParticleActor::AParticleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMesh.Object);
		MeshComponent->SetWorldScale3D(FVector(0.3f)); // Scale down
	}
}

void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParticleActor::SetPosition(const FVector& Position)
{
	SetActorLocation(Position);
}
