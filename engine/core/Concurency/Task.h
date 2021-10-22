#ifndef TASK_H
#define TASK_H

#include <functional>
#include <utility>
#include <tuple>
enum class ETaskPriority : int
{
	Low = 0,
	Medium = 1,
	High = 2
};

// What type of thread this task can execute on
enum ETaskType : unsigned short
{
	TT_NONE = 0b00000000,
	TT_WORKER = 0b00000001,
	TT_RENDERER = 0b00000010,
};


class Task
{
private:

	std::function<void()> F_Function;
	ETaskPriority E_Priority;

public:

	Task();
	Task(ETaskPriority newPriority);
	Task(std::shared_ptr<Task> copyTask);
	~Task();

	void RunTask();

	inline void SetPriority(ETaskPriority newPriority) { E_Priority = newPriority; };
	inline ETaskPriority GetPriority() const { return E_Priority; }

	/*Lamda Explanation:
			std::forward is basicly a wrapper for static_cast<Func&&>(func)
			std::make_tuple helps to automaticly deduce argument types
			std::apply binds the arguments to the function
		*/

	bool operator()(const std::shared_ptr<Task>& t1, const std::shared_ptr<Task>& t2)
	{
		return static_cast<int>(t1->GetPriority()) > static_cast<int>(t2->GetPriority());
	}



#pragma region Template Functions

	

	template<typename Func, typename... Args>
	inline void SetTask(Func&& func, Args&&... args)
	{
		
		F_Function = [func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
		{
			std::apply(func, std::move(args));
		};
	}

	template<typename Func>
	inline void SetTask(Func&& func)
	{
		F_Function = [func = std::forward<Func>(func)]() mutable {};
	}

	inline void SetTask(std::function<void()> newFunc);

#pragma endregion


	
};
#endif
