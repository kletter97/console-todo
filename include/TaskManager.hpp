#ifndef TM_H
#define TM_H

#include "Folder.hpp"
#include "CTDInfo.hpp"
#include <vector>

class TaskManager
{
    private:
        std::vector<Folder*> Folders;
        CTDInfo ctdinfo;
        void printDelimeter(const int& nameLength, const int& mode) const;
    public:
        TaskManager();
        TaskManager(const bool mode);
        void parseInput(const std::string& input);
        void readTasks();
        void saveTasks();
        Task* getTaskByName(const std::string& taskname) const;
        Folder* getFolderByName(const std::string& foldername) const;
        Folder* getFolderByTask(Task*& targetTask) const;
        std::vector<Task*> getAllTasks() const;
        std::vector<Task*> getTasksByDate(const int& day, const int& month, const int& year) const;
        std::vector<Task*> getTasksFromFolder(const std::string& folderName) const;
        std::vector<Task*> getTasksFromFolder(Folder* targetFolder) const;
        void moveTaskToFolder(Task* targetTask, Folder* newFolder);
        void printAllTasks() const;
        void printTasks(const std::vector<Task*>& tasks) const;
        void printLogo() const;
};

#endif