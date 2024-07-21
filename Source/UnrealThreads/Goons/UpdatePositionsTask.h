#pragma once

#include "CoreMinimal.h"
#include "ITask.h"
#include "Particle.h"


class AGoonActor;

class UpdatePositionsTask : public ITask
{
public:
	UpdatePositionsTask(AGoonActor* InGoonActor)
		: GoonActor(InGoonActor) {}

	virtual void Execute() override;

private:
	UPROPERTY()
	AGoonActor* GoonActor;
};
