#include "ThreadHandler.h"
#include "Thread.h"
std::unique_ptr<ThreadHandler> ThreadHandler::U_ThreadHandlerInstance = std::unique_ptr<ThreadHandler>();


ThreadHandler::ThreadHandler()
{
	for (int thread = 0; thread < 2; thread++)
	{
		V_Threads.push_back(std::make_shared<Thread>());
	}

	V_Threads.push_back(std::make_shared<Thread>(EThreadPriority::THREAD_PRIORITYHIGHEST,EThreadType::TH_RENDERER));
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

void ThreadHandler::AddTask(std::shared_ptr<Task> newTask, ETaskFlags taskFlag)
{
	//std::unique_lock<std::mutex> lock(m_HandlerMutex);
	PQ_Tasks.emplace(newTask);


	if (taskFlag & ETaskFlags::TF_WAITFORFINISH)
	{
		RunTasksWithWait();
	}
	else { RunTasks(); }
	
}

void ThreadHandler::AddStrand(std::shared_ptr<Strand> newStrand)
{
	std::scoped_lock<std::mutex> lock(m_HandlerMutex);
	Q_Strands.push(newStrand);

	RunTasks();
}

void ThreadHandler::AddStrand(std::vector<std::shared_ptr<Task>> linkedTasks)
{
	std::scoped_lock<std::mutex> lock(m_HandlerMutex);
	std::shared_ptr<Strand> newStrand =  std::make_shared<Strand>(linkedTasks);
	Q_Strands.push(newStrand);

	RunTasks();
}


void ThreadHandler::WaitForRender()
{
	for (auto& thread : V_Threads)
	{
		if(~thread->GetThreadType() & TH_RENDERER)
			continue;


		if(thread->IsCurrentTaskNull())
			break;

		std::unique_lock<std::mutex> waitLock(thread->GetMutex());

		thread->GetConditionVar().wait(waitLock, [&thread] { return  thread->IsCurrentTaskNull(); });
		break;
	}
}

void ThreadHandler::RunTasks()
{
	
	for (auto& thread : V_Threads)
	{
		if(!thread->IsCurrentTaskNull())
			continue;

		

		if (!Q_Strands.empty())
		{

			//std::unique_lock<std::mutex> strandWaitLock(m_HandlerMutex);

			std::shared_ptr<Strand> currentStrand = Q_Strands.front();
			bool nextThread = false;
			for (auto task : currentStrand->linkedTasks)
			{
				std::unique_lock<std::mutex> taskWaitLock(thread->GetMutex());

				if (task->GetTaskType() != static_cast<ETaskType>(thread->GetThreadType()) && !task->HasBeenCompleted())
				{
					nextThread = true;
					break;
				}

				if (!task->HasBeenCompleted())
				{
					thread->SetNewTask(task);
					thread->GetConditionVar().wait(taskWaitLock, [thread] { return  thread->IsCurrentTaskNull(); });
				}
			}

			

			if(nextThread)
				continue;

			Q_Strands.pop();
			break;
		}


		if(PQ_Tasks.empty())
			break;

		if (PQ_Tasks.top()->GetTaskType() != static_cast<ETaskType>(thread->GetThreadType()))
			continue;

		thread->SetNewTask(PQ_Tasks.top());
		PQ_Tasks.pop();
		
	}

}

void ThreadHandler::RunTasksWithWait()
{
	for (auto& thread : V_Threads)
	{
		if (!thread->IsCurrentTaskNull())
			continue;

		if (PQ_Tasks.empty())
			break;

		if (PQ_Tasks.top()->GetTaskType() != static_cast<ETaskType>(thread->GetThreadType()))
			continue;

		std::unique_lock<std::mutex> taskWaitLock(thread->GetMutex());

		thread->SetNewTask(PQ_Tasks.top());
		PQ_Tasks.pop();

		thread->GetConditionVar().wait(taskWaitLock, [thread] { return  thread->IsCurrentTaskNull(); });
	}
}
