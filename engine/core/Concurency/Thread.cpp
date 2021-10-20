#include "Thread.h"
#include "Task.h"

#include <windows.h>
#include <chrono>

#include <processthreadsapi.h>

//#define WIN32_LEAN_AND_MEAN      


Thread::Thread() : computeTime(0.0f), isActive(true), priority(EThreadPriority::THREAD_PRIORITYIDLE)
{
	thread = std::thread(&Thread::ExecuteCurrentTask, this);
	
	
	SetPriority(priority);

	
}

Thread::~Thread()
{

}

void Thread::SetNewTask(std::shared_ptr<Task> newTask)
{
	currentTask = newTask;
}

void Thread::ExecuteCurrentTask()
{
	while (isActive)
	{
		if (currentTask != nullptr)
		{
			std::unique_lock<std::mutex>  uLock(mutex);
			currentTask->RunTask();
			currentTask = nullptr;
			condition.notify_one();
		}
		else
		{
			// Make sure that this only happens once if it recently got called
			//SetPriority(EThreadPriority::THREAD_PRIORITYIDLE);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
}

void Thread::JoinThread()
{
	isActive = false;

	if (thread.joinable())
	{
		thread.join();
	}
}

bool Thread::IsCurrentTaskNull() const
{
	return currentTask == nullptr;
}

std::mutex& Thread::GetMutex() 
{
	return mutex;
}

std::condition_variable& Thread::GetConditionVar() 
{
	return condition;
}

void Thread::SetPriority(EThreadPriority newPriority)
{

	std::scoped_lock<std::mutex> lock(mutex);
	priority = newPriority;
	SetThreadPriority(thread.native_handle(), -15);
}
