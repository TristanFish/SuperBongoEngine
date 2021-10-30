#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include "Task.h"


// What type of thread this task can execute on
enum ETaskFlags : unsigned short
{
	TF_NONE = 0b00000000,
	TF_WAITFORFINISH = 0b0000001,
};

class Thread;
struct Strand
{
	std::vector<std::shared_ptr<Task>> linkedTasks;


	inline Strand(std::vector<std::shared_ptr<Task>> linkedTasks_)
	{
		linkedTasks = linkedTasks_;
	}
};

class ThreadHandler
{



private:

	std::vector<std::shared_ptr<Thread>> V_Threads;
	std::priority_queue<std::shared_ptr<Task>,std::vector<std::shared_ptr<Task>> ,Task> PQ_Tasks;
	std::queue<std::shared_ptr<Strand>> Q_Strands;

	std::mutex m_HandlerMutex;
	std::condition_variable m_ConditionVar;

	static std::unique_ptr<ThreadHandler> U_ThreadHandlerInstance;
	friend std::default_delete<ThreadHandler>;

public:

	ThreadHandler();
	~ThreadHandler();

	static ThreadHandler* GetInstance();



#pragma region Adding Tasks

	template<typename Func, typename... Args>
	inline void AddTask(Func&& func, Args&&... args)
	{
		std::shared_ptr<Task> newTask;

		newTask->SetTask(func, args);

		PQ_Tasks.push(newTask);
	}


	void AddTask(std::shared_ptr<Task> newTask, ETaskFlags taskFlag = ETaskFlags::TF_NONE);

	void AddStrand(std::shared_ptr<Strand> newStrand);

	void AddStrand(std::vector<std::shared_ptr<Task>> linkedTasks);

#pragma endregion


#pragma region Syncronization

	void RunTasks();

	void RunTasksWithWait();

	void WaitForRender();

#pragma endregion
};
#endif

