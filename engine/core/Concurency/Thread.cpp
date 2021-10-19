#include "Thread.h"
#include "Task.h"

#include <windows.h>

#include <processthreadsapi.h>

//#define WIN32_LEAN_AND_MEAN      


Thread::Thread() : computeTime(0.0f), isActive(true), priority(EThreadPriority::THREAD_PRIORITYNORMAL)
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
		std::unique_lock<std::mutex>  uLock(mutex, std::defer_lock);
		if (currentTask != nullptr)
		{
			//condition.wait(uLock);
			uLock.lock();



			currentTask->RunTask();
			currentTask = nullptr;
			uLock.unlock();
			//condition.notify_all();
		}
		else
		{
			// Make sure that this only happens once if it recently got called
			uLock.lock();
			SetPriority(EThreadPriority::THREAD_PRIORITYIDLE);
			uLock.unlock();
		}
	}
}

void Thread::JoinThread()
{
	isActive = false;

	thread.join();
}

bool Thread::IsCurrentTaskNull() const
{
	return currentTask == nullptr;
}

void Thread::SetPriority(EThreadPriority newPriority)
{
	priority = newPriority;

	//SetThreadPriority(thread.native_handle(), static_cast<int>(priority));
}
