#include "Task.h"








	

Task::Task(): E_Priority(ETaskPriority::Low)
{

}
Task::Task(ETaskPriority newPriority) : E_Priority(newPriority)
{

}

Task::Task(std::shared_ptr<Task> copyTask) : E_Priority(copyTask->E_Priority), F_Function(copyTask->F_Function)
{


}

Task::~Task()
{

}

void Task::RunTask()
{
	F_Function();
}

void Task::SetTask(std::function<void()> newFunc)
{
	F_Function = newFunc;
}
