#include "Thread.h"
#include "Task.h"

#include "core/CoreEngine.h"
#include "graphics/Window.h"
#include <windows.h>
#include <chrono>
#include <processthreadsapi.h>
     


Thread::Thread() : F_ComputeTime(0.0f), isActive(true), E_Priority(EThreadPriority::THREAD_PRIORITYNORMAL), E_ThreadType(TH_GENERAL), once_flag(false)
{

	SetPriority(E_Priority);


	T_Thread = std::thread(&Thread::ExecuteCurrentTask, this);
	
}

Thread::Thread(EThreadPriority priority, EThreadType threadType) : F_ComputeTime(0.0f), isActive(true), once_flag(false)
{

	SetPriority(priority);

	E_ThreadType = threadType;

	T_Thread = std::thread(&Thread::ExecuteCurrentTask, this);
}

Thread::~Thread()
{

}

void Thread::SetNewTask(std::shared_ptr<Task> newTask)
{
	T_CurrentTask = newTask;
}

void Thread::ExecuteCurrentTask()
{
	// Make sure that the opengl/sdl thread is changed before we make any calls
	if (E_ThreadType & EThreadType::TH_RENDERER && !once_flag)
	{
		CoreEngine::GetInstance()->GetWindow()->ChangeSDL_GL_Thread();
		once_flag = true;
	}

	while (isActive)
	{
		if (T_CurrentTask != nullptr)
		{

			std::unique_lock<std::mutex>  uLock(M_Mutex);

			T_CurrentTask->RunTask();
			

			T_CurrentTask = nullptr;
			C_ConditionVar.notify_one();
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

	if (T_Thread.joinable())
	{
		T_Thread.join();
	}
}

bool Thread::IsCurrentTaskNull() const
{
	return T_CurrentTask == nullptr;
}

std::mutex& Thread::GetMutex() 
{
	return M_Mutex;
}

std::condition_variable& Thread::GetConditionVar() 
{
	return C_ConditionVar;
}

EThreadType Thread::GetThreadType() const
{
	return E_ThreadType;
}

void Thread::SetPriority(EThreadPriority newPriority)
{

	std::scoped_lock<std::mutex> lock(M_Mutex);
	E_Priority = newPriority;
	SetThreadPriority(T_Thread.native_handle(), static_cast<int>(E_Priority));
}
