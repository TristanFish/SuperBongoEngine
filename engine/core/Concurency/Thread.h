#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

enum class EThreadPriority : int
{
	THREAD_PRIORITYIDLE = -15,
	THREAD_PRIORITYLOWEST = -2,
	THREAD_PRIORITYNORMAL = 0,
	THREAD_PRIORITYHIGHEST = 2
};

enum EThreadType : unsigned short
{
	TH_GENERAL = 0b00000001,
	TH_UPDATER = 0b00000010,
	TH_RENDERER = 0b00000100,
};


class Task;
class Thread
{
private:
	std::thread T_Thread;

	std::mutex M_Mutex;

	std::condition_variable C_ConditionVar;

	std::shared_ptr<Task> T_CurrentTask;

	bool once_flag;

	EThreadPriority E_Priority;

	EThreadType E_ThreadType;

	float F_ComputeTime;


public:
	std::atomic_bool isActive;

	Thread();
	Thread(EThreadPriority priority, EThreadType threadType);
	~Thread();
	

	void ExecuteCurrentTask();

	void JoinThread();

	bool IsCurrentTaskNull() const;

	std::mutex& GetMutex();
	std::condition_variable& GetConditionVar();

	EThreadType GetThreadType() const;

	void SetNewTask(std::shared_ptr<Task> newTask);
	void SetPriority(EThreadPriority newPriority);
};

#endif
