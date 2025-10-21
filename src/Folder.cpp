#include "Folder.hpp"

Folder::Folder()
{
    name = "Folder-1";
}
Folder::Folder(const std::string inName)
{
    name = inName;
}
std::string Folder::getName() const
{
    return name;
}
std::vector<Task*> Folder::getTasks() const
{
    return Tasks;
}
std::vector<Task*> Folder::getUndoneTasks() const
{
    std::vector<Task*> undoneTasks;
    for(Task* task : Tasks)
    {
        if(!(task->getStatus())) undoneTasks.push_back(task);
    }
    return undoneTasks;
}
void Folder::setName(const std::string newName)
{
    name = newName;
}
void Folder::addTask(Task* newTask)
{
    Tasks.push_back(newTask);
}
void Folder::removeTask(Task* targetTask)
{
    Tasks.erase(std::find(Tasks.begin(), Tasks.end(), targetTask));
}
void Folder::transferTask(const Task& task)
{
    ; //ToDo: transfer task to another folder
}
void Folder::receiveTask(const Task& task)
{
    ; //ToDo: receive task from another folder
}
void Folder::exportJSON(const std::string filename) const
{
    ; //ToDo: export folder to JSON
}
