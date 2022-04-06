#include "Task.h"

#include "Thread.h"

Task::Task(): E_Priority(ETaskPriority::Low), E_TaskType(ETaskType::TT_GENERAL), B_HasBeenCompleted(false)
{

}
Task::Task(ETaskPriority newPriority, ETaskType newType) : E_Priority(newPriority), E_TaskType(newType), B_HasBeenCompleted(false)
{

}

Task::Task(std::shared_ptr<Task> copyTask) : E_Priority(copyTask->E_Priority),E_TaskType(ETaskType::TT_GENERAL), F_Function(copyTask->F_Function), B_HasBeenCompleted(false)
{


}

Task::~Task()
{

}

void Task::RunTask()
{
	F_Function();
	B_HasBeenCompleted = true;
}



void Task::SetTask(std::function<void()> newFunc)
{
	F_Function = newFunc;
}
