#include "UpdatePositionsTask.h"
#include "GoonActor.h"
#include "Async/Async.h"

void UpdatePositionsTask::Execute()
{
	FParticleUpdate Update;
	while (GoonActor->PositionUpdateQueue.Dequeue(Update))
	{
		if (Update.Actor)
		{
			// Queue the position update to run on the game thread
			AsyncTask(ENamedThreads::GameThread, [Update]()
				{
					Update.Actor->SetActorLocation(Update.NewPosition);
				});
		}
	}
}