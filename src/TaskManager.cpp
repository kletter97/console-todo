#include "TaskManager.hpp"
#include <iostream>

TaskManager::TaskManager()
{
    ;
}
TaskManager::TaskManager(bool mode)
{
    // just for testing the class without import/export tasks (those methods are not created so far)
    Task* testTask = new Task;
    Folder* testFolder = new Folder;
    testFolder->addTask(testTask);
    Folders.push_back(testFolder);
}
void TaskManager::parseInput(std::string input)
{
    std::vector<std::string> words;
    int oldi=-1, i;
    for(i=0; i<(int)input.size(); i++)
    {
        if(input[i]==' ')
        {
            words.push_back(input.substr(oldi+1, i-oldi-1));
            oldi = i;
        }
    }
    words.push_back(input.substr(oldi+1));
    if(words[0]=="display")
    {
        std::vector<Task*> tasksForDisplay;
        if(words[1]=="all") printAllTasks();
        else if(words[1]=="date" || words[1]=="folder")
        {
            if(words[1]=="date") tasksForDisplay = getTasksByDate(stoi(words[2]), stoi(words[3]), stoi(words[4]));
            else if(words[1]=="folder") tasksForDisplay = getTasksFromFolder(words[2]);
            printTasks(tasksForDisplay);
        }
        else throw std::invalid_argument("unknown display command: "+words[1]);
    }
    else if(words[0]=="edit")
    {
        Task* targetTask = getTaskByName(words[1]);
        if(words[2]=="name") targetTask->setName(words[3]);
        else if(words[2]=="date")
        {
            if(words[3][0]=='+' || words[3][0]=='-') targetTask->getEndDate().move(stoi(words[3]));
            else
            {
                Date newDate((unsigned)stoi(words[3]), (unsigned)stoi(words[4]), stoi(words[5]));
                targetTask->setEndDate(newDate);
            }
        }
    }
}
void TaskManager::readTasks()
{
    ; //ToDo: read tasks from file
}
void TaskManager::saveTasks()
{
    ; //ToDo: save tasks to file
}
Task* TaskManager::getTaskByName(std::string taskname)
{
    for(Folder* folder : Folders)
    {
        for(Task* task : folder->getTasks())
        {
            if(task->getName() == taskname) return task;
        }
    }
    throw std::invalid_argument("task \""+taskname+"\" not found");
}
std::vector<Task*> TaskManager::getAllTasks()
{
    std::vector<Task*> ret;
    for(Folder* folder : Folders)
    {
        for(Task* task : folder->getTasks())
        {
            ret.push_back(task);
        }
    }
    return ret;
}
std::vector<Task*> TaskManager::getTasksByDate(int day, int month, int year)
{
    std::vector<Task*> ret;
    for(Folder* folder : Folders)
    {
        for(Task* task : folder->getTasks())
        {
            if(task->getEndDate().getDay()==day && task->getEndDate().getMonth()==month && task->getEndDate().getYear()==year) ret.push_back(task);
        }
    }
    return ret;
}
std::vector<Task*> TaskManager::getTasksFromFolder(std::string folderName)
{
    for(Folder* folder : Folders)
    {
        if(folder->getName()==folderName) return folder->getTasks();
    }
    throw std::invalid_argument("folder \""+folderName+"\" not found");
}
void TaskManager::printAllTasks()
{
    // statusLength = 6 ("Status"), any dateLength = 18 ("September DD, YYYY")
    int nameLength=4;
    for(Folder* folder : Folders)
    {
        for(Task* task : folder->getTasks())
        {
            if(task->getName().length() > nameLength) nameLength = task->getName().length();
        }
    }

    //Upper bar output, format: | Name | Status | Due | Created |
    std::cout << "| Name ";
    for(int i=0; i<nameLength-4; ++i) std::cout << " ";
    std::cout << "| Status | Due ";
    for(int i=0; i<15; ++i) std::cout << " ";
    std::cout << "| Created ";
    for(int i=0; i<11; ++i) std::cout << " ";
    std::cout << "|" << std::endl;

    for(Folder* folder : Folders)
    {
        std::cout << folder->getName() << std::endl;
        for(Task* task : folder->getTasks())
        {
            // name output
            std::cout << "| " << task->getName();
            for(int i=0; i<nameLength-task->getName().length()+1; ++i) std::cout << " ";
            // status output
            std::cout << "| ";
            if(task->getStatus()) std::cout << "Done ";
            else std::cout << "Undone ";
            // deadline (endDate) output
            std::cout << "| " << task->getEndDate().print();
            for(int i=0; i<19-(task->getEndDate().print().length()); ++i) std::cout << " ";
            // creation date (startDate) output
            std::cout << "| " << task->getStartDate().print();
            for(int i=0; i<19-(task->getStartDate().print().length()); ++i) std::cout << " ";
            // final "|" output
            std::cout << "| " << std::endl;
        }
    }
}
void TaskManager::printTasks(std::vector<Task*> tasks)
{
    int nameLength = 4;
    for(Task* task : tasks)
    {
        if(task->getName().length() > nameLength) nameLength = task->getName().length();
    }
    //Upper bar output, format: | Name | Status | Due | Created |
    std::cout << "| Name ";
    for(int i=0; i<nameLength-4; ++i) std::cout << " ";
    std::cout << "| Status | Due ";
    for(int i=0; i<15; ++i) std::cout << " ";
    std::cout << "| Created ";
    for(int i=0; i<12; ++i) std::cout << " ";
    std::cout << "|" << std::endl;

    for(Task* task : tasks)
    {
        // name output
        std::cout << "| " << task->getName();
        for(int i=0; i<nameLength-task->getName().length(); ++i) std::cout << " ";
        // status output
        std::cout << "| ";
        if(task->getStatus()) std::cout << "Done";
        else std::cout << "Undone";
        // deadline (endDate) output
        std::cout << "| " << task->getEndDate().print();
        for(int i=0; i<18-(task->getEndDate().print().length()); ++i) std::cout << " ";
        // creation date (startDate) output
        std::cout << "| " << task->getStartDate().print();
        for(int i=0; i<18-(task->getStartDate().print().length()); ++i) std::cout << " ";
        // final "|" output
        std::cout << "| " << std::endl;
    }
}