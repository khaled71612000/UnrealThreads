#include "GoonActor.h"

// Sets default values
AGoonActor::AGoonActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGoonActor::BeginPlay()
{
	Super::BeginPlay();

	// Create runnables with different priorities
	//RunnableGoon* HighPriorityRunnable = new RunnableGoon(TPri_Highest);
	//RunnableGoon* NormalPriorityRunnable = new RunnableGoon(TPri_Normal);
	//RunnableGoon* LowPriorityRunnable = new RunnableGoon(TPri_Lowest);

	RunnableGoons.Add(new RunnableGoon(TPri_Highest));
	RunnableGoons.Add(new RunnableGoon(TPri_Normal));
	RunnableGoons.Add(new RunnableGoon(TPri_Lowest));

	// Start threads with different priorities
	Threads.Add(FRunnableThread::Create(RunnableGoons[0], TEXT("HighPriorityThread"), 0, TPri_Highest));
	Threads.Add(FRunnableThread::Create(RunnableGoons[1], TEXT("NormalPriorityThread"), 0, TPri_Normal));
	Threads.Add(FRunnableThread::Create(RunnableGoons[2], TEXT("LowPriorityThread"), 0, TPri_Lowest));
}
// Called every frame
void AGoonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoonActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	/*If the editor freezes when EndPlay is called,
	it's likely because the thread is not terminating properly. This can happen if the thread is still running when you try to kill it.
	To ensure that the thread stops correctly, you should give it some time to exit gracefully before forcefully killing it.*/
	for (RunnableGoon* Runnable : RunnableGoons)
	{
		if (Runnable)
		{
			Runnable->Stop();
		}
	}

	/*In Unreal Engine, objects that are not managed by the Garbage Collector(GC) need to be explicitly managed to avoid memory leaks.
	Actors and UObjects typically need to be managed by the GC.However,
	FRunnable and FRunnableThread are not UObjects, so they are not managed by the GC.This means you must manually handle their memory management.*/

	for (FRunnableThread* Thread : Threads)
	{
		if (Thread)
		{
			Thread->WaitForCompletion();
			delete Thread;
			Thread = nullptr;
		}
	}

	for (RunnableGoon* Runnable : RunnableGoons)
	{
		if (Runnable)
		{
			delete Runnable;
			Runnable = nullptr;
		}
	}

	Super::EndPlay(EndPlayReason);
	}