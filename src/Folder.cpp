#include "Folder.hpp"

Folder::Folder()
{
    name = "Folder-1";
}
Folder::Folder(const std::string inName, bool mode)
{
    if(!mode) name = inName;
    else; //ToDo: import from JSON
}
std::string Folder::getName() const
{
    return name;
}
std::vector<Task*> Folder::getTasks() const
{
    return Tasks;
}
void Folder::setName(const std::string newName)
{
    name = newName;
}
void Folder::addTask(Task* newTask)
{
    Tasks.push_back(newTask);
}
void Folder::removeTask(const std::string taskName)
{
    ; //
}
Task* Folder::getTask(const std::string taskName) const
{
    ; //
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