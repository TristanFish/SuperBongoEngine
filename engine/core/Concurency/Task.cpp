#include "Task.h"








	

Task::Task(): priority(EPriority::Low)
{

}
Task::Task(EPriority newPriority) : priority(newPriority)
{

}
Task::~Task()
{

}

void Task::RunTask()
{
	function();
}

void Task::SetTask(std::function<void()> newFunc)
{
	function = newFunc;
}
