#pragma once

#include "CoreMinimal.h"
#include "ITask.h"
#include "Particle.h"
#include "TaskNode.h"
#include "ParticleShakeTask.h"

class AGoonActor;

class AddShakeTasksTask : public ITask
{
public:
	AddShakeTasksTask(AGoonActor* InGoonActor)
		: GoonActor(InGoonActor) {}

	virtual void Execute() override;

private:
	UPROPERTY()
	AGoonActor* GoonActor;
};
