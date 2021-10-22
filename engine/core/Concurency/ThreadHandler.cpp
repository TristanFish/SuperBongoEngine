#include "ThreadHandler.h"
#include "Thread.h"
std::unique_ptr<ThreadHandler> ThreadHandler::U_ThreadHandlerInstance = std::unique_ptr<ThreadHandler>();


ThreadHandler::ThreadHandler()
{
	for (int thread = 0; thread < 4; thread++)
	{
		V_Threads.push_back(std::make_shared<Thread>());
	}

	V_Threads.push_back(std::make_shared<Thread>(EThreadPriority::THREAD_PRIORITYNORMAL,EThreadType::TH_RENDERER));
}

ThreadHandler::~ThreadHandler()
{
	for (auto thread : V_Threads)
	{
		thread->JoinThread();
	}
}

ThreadHandler* ThreadHandler::GetInstance()
{
	if (U_ThreadHandlerInstance == nullptr)
	{
		U_ThreadHandlerInstance.reset(new ThreadHandler);
	}
	return U_ThreadHandlerInstance.get();
}

void ThreadHandler::AddTask(std::shared_ptr<Task> newTask)
{
	//std::unique_lock<std::mutex> lock(m_HandlerMutex);
	PQ_Tasks.emplace(newTask);

	RunThreads();
}

void ThreadHandler::AddStrand(std::shared_ptr<Strand> newStrand)
{
	std::scoped_lock<std::mutex> lock(m_HandlerMutex);
	Q_Strands.push(newStrand);

	RunThreads();
}

void ThreadHandler::AddStrand(std::vector<std::shared_ptr<Task>> linkedTasks)
{
	std::scoped_lock<std::mutex> lock(m_HandlerMutex);
	std::shared_ptr<Strand> newStrand =  std::make_shared<Strand>(linkedTasks);
	Q_Strands.push(newStrand);

	RunThreads();
}


void ThreadHandler::RunThreads()
{
	
	for (auto thread : V_Threads)
	{
		if(!thread->IsCurrentTaskNull())
			continue;


		if (!Q_Strands.empty())
		{
			std::shared_ptr<Strand> currentStrand = Q_Strands.front();

			for (auto task : currentStrand->linkedTasks)
			{
				std::unique_lock<std::mutex> waitLock(thread->GetMutex());
				thread->SetNewTask(task);
				thread->GetConditionVar().wait(waitLock, [&thread] { return  thread->IsCurrentTaskNull(); });
			}

			Q_Strands.pop();
			break;
		}


		if(PQ_Tasks.empty())
			break;
		thread->SetNewTask(PQ_Tasks.top());
		PQ_Tasks.pop();
		
	}

}
