a# Unreal Engine Multithreading Project

## Overview

This project showcases the implementation of various multithreading techniques in Unreal Engine, focusing on efficient task management and execution. The project demonstrates the use of thread pools, task graphs, and custom runnable classes to handle concurrency and improve performance.

## Classes

### ThreadPoolManager

The `ThreadPoolManager` class manages a pool of worker threads, efficiently distributing tasks across multiple threads to balance the load.

#### Key Methods
- `CreateThreads(int32 NumThreads)`: Creates and starts the specified number of threads.
- `StopAll()`: Signals all threads to stop and waits for their completion.
- `AddThreads(int32 AdditionalThreads)`: Adds additional threads to the pool.

### UpdatePositionsTask

The `UpdatePositionsTask` class updates actor positions on the game thread based on data processed in background threads.

#### Key Methods
- `Execute()`: Dequeues position updates and schedules them to run on the game thread.

### RunnableGoon

The `RunnableGoon` class implements a custom runnable to execute tasks on separate threads with specific core affinity settings.

#### Key Methods
- `Init()`: Initializes the runnable.
- `Run()`: Main execution loop for the thread.
- `Stop()`: Requests the thread to stop.
- `AddTask(TSharedPtr<ITask> Task)`: Adds a task to the queue for execution.
- `ExecuteTasks()`: Executes tasks from the queue.

### PrimeCalculationTask

The `PrimeCalculationTask` class calculates prime numbers in a background thread, demonstrating the use of thread-safe queues and critical sections.

#### Key Methods
- `Execute()`: Performs the prime number calculation and enqueues the result if it is prime.
- `IsPrime(int32 Num)`: Checks if a number is prime.

### ParticleShakeTask

The `ParticleShakeTask` class defines a task for shaking particles, showcasing the use of task-based multithreading.

#### Key Methods
- `Execute()`: Executes the shake action on the particle.

### TaskGraphManager

The `TaskGraphManager` class manages a graph of dependent tasks, ensuring they are executed in the correct order using a task graph system.

#### Key Methods
- `AddTask(TSharedPtr<FTaskNode> Node)`: Adds a new task to the task graph.
- `Execute()`: Executes the tasks in the graph.
- `HasPendingTasks()`: Checks if there are pending tasks.

### GoonActor

The `GoonActor` class integrates all components, demonstrating how tasks are scheduled and executed within an Unreal Engine actor.

#### Key Methods
- `BeginPlay()`: Initializes the thread pool and task graph, and starts the execution.
- `InitializeParticles()`: Initializes particles and schedules shake tasks.
- `Tick(float DeltaTime)`: Regularly schedules update positions and executes tasks.
- `ScheduleShakeTasks()`: Schedules shake tasks.
- `ScheduleUpdatePositions()`: Schedules position update tasks.
- `EndPlay(const EEndPlayReason::Type EndPlayReason)`: Cleans up the thread pool and task graph on end play.

## Features

- **Efficient Task Management**: Utilizes thread pools and task graphs to manage and execute tasks concurrently.
- **Thread-Safe Operations**: Ensures thread safety using critical sections and atomic operations.
- **Custom Runnable Classes**: Implements custom runnable classes to execute tasks on separate threads with specific core affinities.
- **Background Calculations**: Demonstrates background calculations like prime number checking and particle shaking.
- **Task Scheduling**: Schedules and executes tasks in a structured manner within Unreal Engine actors.

## Development Process

1. **Initialize Properties**: Set up the thread pool, task graph, and other necessary properties.
2. **Task Implementation**: Implement tasks such as `PrimeCalculationTask` and `ParticleShakeTask`.
3. **Runnable Implementation**: Develop custom runnable classes like `RunnableGoon` to handle task execution.
4. **Task Graph Management**: Manage task dependencies and execution order using `TaskGraphManager`.
5. **Actor Integration**: Integrate all components into an Unreal Engine actor (`GoonActor`).

## Credits

This project is inspired by and based on various multithreading techniques and examples from the Unreal Engine community.

![image](https://github.com/user-attachments/assets/498dcfc9-04ee-44e5-bf65-1852d5e084d7)
![image](https://github.com/user-attachments/assets/eebfc610-aca4-4b31-88c8-e895b2b27270)
![image](https://github.com/user-attachments/assets/a6648b9a-1911-4b93-9caa-5d2a7553fa21)
![image](https://github.com/user-attachments/assets/72e21a69-9e46-46b4-99c1-a7aae166f91d)
![image](https://github.com/user-attachments/assets/3636d08a-81dd-4af0-91cc-875ade9942d2)


