#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleActor.generated.h"

UCLASS()
class UNREALTHREADS_API AParticleActor : public AActor
{
	GENERATED_BODY()

public:
	AParticleActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetPosition(const FVector& Position);
};
