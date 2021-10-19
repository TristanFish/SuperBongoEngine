#ifndef TASK_H
#define TASK_H

#include <functional>
#include <utility>
#include <tuple>
enum class EPriority : int
{
	Low = 0,
	Medium = 1,
	High = 2
};


class Task
{
private:

	std::function<void()> function;
	EPriority priority;

public:

	Task();
	Task(EPriority newPriority);
	~Task();

	void RunTask();

	inline void SetPriority(EPriority newPriority) { priority = newPriority; };
	inline EPriority GetPriority() const { return priority; }

	/*Lamda Explanation:
			std::forward is basicly a wrapper for static_cast<Func&&>(func)
			std::make_tuple helps to automaticly deduce argument types
			std::apply binds the arguments to the function
		*/

	bool operator()(const std::shared_ptr<Task>& t1, const std::shared_ptr<Task>& t2)
	{
		return static_cast<int>(t1->GetPriority()) > static_cast<int>(t2->GetPriority());
	}

	template<typename Func, typename... Args>
	inline void SetTask(Func&& func, Args&&... args)
	{
		
		function = [func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
		{
			std::apply(func, std::move(args));
		};
	}

	template<typename Func>
	inline void SetTask(Func&& func)
	{
		function = [func = std::forward<Func>(func)]() mutable {};
	}

	inline void SetTask(std::function<void()> newFunc);
	
};
#endif
