#include "AddShakeTasksTask.h"
#include "GoonActor.h"

void AddShakeTasksTask::Execute()
{
	TArray<TSharedPtr<FParticle>> LocalParticles;

	{
		FScopeLock Lock(&GoonActor->ParticlesCriticalSection); // Lock the critical section
		LocalParticles = GoonActor->Particles; // Copy the particles array
	}

	for (TSharedPtr<FParticle> Particle : LocalParticles)
	{
		TSharedPtr<ITask> ShakeTask = MakeShareable(new ParticleShakeTask(Particle));
		TSharedPtr<FTaskNode> TaskNode = MakeShareable(new FTaskNode(ShakeTask));
		GoonActor->TaskGraph->AddTask(TaskNode);
	}
}

