#ifndef FOLDER_H
#define FOLDER_H

#include "Task.hpp"
#include <vector>
#include <memory>
#include <string>

class Folder
{
    private:
        std::string name;
        std::vector<Task*> Tasks;
    public:
        Folder();
        Folder(const std::string inName, bool mode);
        std::string getName() const;
        std::vector<Task*> getTasks() const;
        void setName(const std::string newName);
        void addTask(Task* newTask);
        void removeTask(const std::string taskName);
        Task* getTask(const std::string taskName) const;
        void transferTask(const Task& task);
        void receiveTask(const Task& task);
        void exportJSON(const std::string filename) const;
};

#endif