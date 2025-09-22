#ifndef TM_H
#define TM_H

#include "Folder.hpp"
#include <vector>

class TaskManager
{
    private:
        std::vector<Folder*> Folders;
        void printDelimeter(int nameLength, int mode);
    public:
        TaskManager();
        TaskManager(bool mode);
        void parseInput(std::string input);
        void readTasks();
        void saveTasks();
        Task* getTaskByName(std::string taskname);
        std::vector<Task*> getAllTasks();
        std::vector<Task*> getTasksByDate(int day, int month, int year);
        std::vector<Task*> getTasksFromFolder(std::string folderName);
        void printAllTasks();
        void printTasks(std::vector<Task*> tasks);
};

#endif