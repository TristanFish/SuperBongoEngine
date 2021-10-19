#include "ThreadHandler.h"
#include "Thread.h"
std::unique_ptr<ThreadHandler> ThreadHandler::threadHandlerInstance = std::unique_ptr<ThreadHandler>();


ThreadHandler::ThreadHandler()
{
	for (int thread = 0; thread < std::thread::hardware_concurrency() - 1; thread++)
	{
		threads.push_back(std::make_shared<Thread>());
	}
}

ThreadHandler::~ThreadHandler()
{
	for (auto thread : threads)
	{
		thread->JoinThread();
	}
}

ThreadHandler* ThreadHandler::GetInstance()
{
	if (threadHandlerInstance == nullptr)
	{
		threadHandlerInstance.reset(new ThreadHandler);
	}
	return threadHandlerInstance.get();
}

void ThreadHandler::AddTask(std::shared_ptr<Task> newTask)
{
	tasks.push(newTask);
}

void ThreadHandler::RunThreads()
{
	
	for (auto thread : threads)
	{
		if(!thread->IsCurrentTaskNull())
			continue;


		if (!strands.empty())
		{
			std::shared_ptr<Strand> currentStrand = strands.front();

			for (auto task : currentStrand->linkedTasks)
			{
				thread->SetNewTask(task);

				while (!thread->IsCurrentTaskNull()) {}
			}


		}


		if(tasks.empty())
			break;
		thread->SetNewTask(tasks.top());
		tasks.pop();
		
	}

}
