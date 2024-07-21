#pragma once

#include "CoreMinimal.h"
#include "ITask.h"
#include "Particle.h"

class ParticleShakeTask : public ITask
{
public:
	ParticleShakeTask(TSharedPtr<FParticle> InParticle)
		: Particle(InParticle) {}

	virtual void Execute() override
	{
		if (Particle.IsValid())
		{
			Particle->Shake();
		}
	};

private:
	TSharedPtr<FParticle> Particle;
};
