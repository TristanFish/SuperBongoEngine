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

class Task;
class Thread
{
private:
	std::thread thread;

	std::mutex mutex;

	std::condition_variable condition;

	std::shared_ptr<Task> currentTask;

	EThreadPriority priority;

	float computeTime;


public:
	std::atomic_bool isActive;

	Thread();
	~Thread();
	

	void ExecuteCurrentTask();

	void JoinThread();

	bool IsCurrentTaskNull() const;

	std::mutex& GetMutex();
	std::condition_variable& GetConditionVar();

	void SetNewTask(std::shared_ptr<Task> newTask);
	void SetPriority(EThreadPriority newPriority);
};

#endif
