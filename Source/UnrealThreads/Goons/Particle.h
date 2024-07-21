#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "../ParticleActor.h"

struct FParticleUpdate
{
	AParticleActor* Actor;
	FVector NewPosition;
};

class FParticle
{
public:
	FParticle(FVector InPosition, AParticleActor* InActor, TQueue<FParticleUpdate, EQueueMode::Mpsc>& InUpdateQueue)
		: Position(InPosition), Actor(InActor), UpdateQueue(InUpdateQueue) {}

	void Shake()
	{
		// Increase the shake intensity by multiplying the random range
		FVector NewPosition = Position + FVector(FMath::FRandRange(-10.0f, 10.0f), FMath::FRandRange(-10.0f, 10.0f), FMath::FRandRange(-10.0f, 10.0f));
		UpdateQueue.Enqueue(FParticleUpdate{ Actor, NewPosition });
		Position = NewPosition; // Update the current position
	}

	FVector GetPosition() const { return Position; }

private:
	FVector Position;
	AParticleActor* Actor;
	TQueue<FParticleUpdate, EQueueMode::Mpsc>& UpdateQueue; // Queue for position updates
};
