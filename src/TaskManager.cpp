#include "TaskManager.hpp"
#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>

namespace fs = std::filesystem;

TaskManager::TaskManager()
{
    try {readTasks();}
    catch(...) {std::cout << "ERROR: Tasks files are corrupted, reading failed. Program has been stopped.\n"; std::exit(1);}
    if(Folders.size()==0)
    {
        std::cout << "NOTE: No tasks found, created an empty folder \"General\".\n";
        Folder* General = new Folder("General");
        Folders.push_back(General);
    }
}
TaskManager::TaskManager(bool mode)
{
    // just for testing the class without import/export tasks (those methods are not created so far)
    Task* testTask = new Task;
    Folder* testFolder1 = new Folder;
    Folder* testFolder2 = new Folder;
    testFolder1->addTask(testTask);
    testFolder2->addTask(testTask);
    Folders.push_back(testFolder1);
    Folders.push_back(testFolder2);
}
void TaskManager::parseInput(const std::string& input)
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
            if(words[1]=="date") tasksForDisplay = getTasksByDate(stoi(words[2]), stoi(words[3])-1, stoi(words[4]));
            else if(words[1]=="folder") tasksForDisplay = getTasksFromFolder(words[2]);
            printTasks(tasksForDisplay);
        }
        else throw std::invalid_argument("unknown \"display\" argument: "+words[1]);
    }
    else if(words[0]=="edit")
    {
        Task* targetTask = getTaskByName(words[1]);
        if(words[2]=="name") targetTask->setName(words[3]);
        else if(words[2]=="date")
        {
            if(words[3][0]=='+' || words[3][0]=='-') targetTask->getEndDate()->move(stoi(words[3]));
            else
            {
                Date newDate((unsigned)stoi(words[3]), (unsigned)stoi(words[4]), stoi(words[5]));
                targetTask->setEndDate(newDate);
            }
        }
        else throw std::invalid_argument("unknown \"edit\" argument: "+words[1]);
    }
    else if(words[0]=="new")
    {
        if(words[1]=="task")
        {
            Folder* targetFolder = getFolderByName(words[2]);
            Task* newTask = new Task;
            newTask->setName(words[3]);
            Date* newDate = new Date((unsigned)stoi(words[4]), (unsigned)stoi(words[5]), stoi(words[6]));
            newTask->setEndDate(*newDate);
            targetFolder->addTask(newTask);
        }
        else if(words[1]=="folder")
        {
            Folder* newFolder = new Folder(words[2]);
            Folders.push_back(newFolder);
        }
        else throw std::invalid_argument("unknown \"new\" argument: "+words[1]);
    }
    else if(words[0]=="delete")
    {
        if(words[1]=="task")
        {
            Task* newTask = getTaskByName(words[2]);
            Folder* targetFolder = getFolderByTask(newTask);
            targetFolder->removeTask(newTask);
        }
        else if(words[1]=="folder")
        {
            Folder* targetFolder = getFolderByName(words[2]);
            Folders.erase(std::find(Folders.begin(), Folders.end(), targetFolder));
            std::vector<Task*> targetFolderTasks = getTasksFromFolder(targetFolder);
            for(Task* deletableTask : targetFolderTasks) delete deletableTask;
            delete targetFolder;
        }
        else if(words[1]=="done")
        {
            if(words[2]=="all")
            {
                std::vector<Task*> allTasks = getAllTasks();
                for(Task* task : allTasks) if(task->getStatus()==true)
                {
                    Folder* folder = getFolderByTask(task);
                    folder->removeTask(task);
                }
            }
            else
            {
                Folder* folder = getFolderByName(words[2]);
                for(Task* task : folder->getTasks()) if(task->getStatus()==true)
                {
                    Folder* folder = getFolderByTask(task);
                    folder->removeTask(task);
                }
            }
        }
        else throw std::invalid_argument("unknown \"delete\" argument: "+words[1]);
    }
    else if(words[0]=="done")
    {
        Task* targetTask = getTaskByName(words[1]);
        targetTask->setStatus(true);
    }
    else if(words[0]=="undone")
    {
        Task* targetTask = getTaskByName(words[1]);
        targetTask->setStatus(false);
    }
    else if(words[0]=="about") std::cout << ctdinfo.getInfoText() << "\n";
    else if(words[0]=="help") std::cout << ctdinfo.getHelp(words[1]) << "\n";
    else if(words[0]=="exit")
    {
        std::cout << "saving your tasks..." << "\n";
        saveTasks();
        std::exit(0);
    }
    else throw std::invalid_argument("unknown command: "+words[0]);
}
void TaskManager::readTasks()
{
    std::ifstream list("tasks/list.ctdfs"), currentFile;
    std::string currentFileName, currentStr, fieldName, fieldVal;
    Folder* currentFolder;
    Task* currentTask;
    bool flag = 0;
    if(list.is_open()) while (std::getline(list, currentFileName))
        {
            currentFile.open(currentFileName);
            if(currentFile.is_open())
            {
                std::cout << "successfully opened " << currentFileName << std::endl;
                currentFolder = new Folder;
                Folders.push_back(currentFolder);
                while (std::getline(currentFile, currentStr))
                {
                    if(currentStr=="next")
                    {
                        currentTask = new Task;
                        currentFolder->addTask(currentTask);
                    }
                    else
                    {
                        flag = 0;
                        for(int i=0; i<(int)currentStr.size(); i++)
                        {
                            if(currentStr[i]=='=')
                            {
                                fieldName = currentStr.substr(0, i);
                                fieldVal = currentStr.substr(i+1);
                                flag = 1;
                            }
                        }
                        if(!flag) throw std::invalid_argument("File \""+currentFileName+"\" contains invalid string \""+currentStr+"\"");
                        if(fieldName=="folder") currentFolder->setName(fieldVal);
                        else if(fieldName=="name") currentTask->setName(fieldVal);
                        else if(fieldName=="status") currentTask->setStatus(stoi(fieldVal));
                        else if(fieldName=="sdate")
                        {
                            Date date(stoi(fieldVal.substr(0,2)), stoi(fieldVal.substr(2,2)), stoi(fieldVal.substr(4)));
                            currentTask->setStartDate(date);
                        }
                        else if(fieldName=="edate")
                        {
                            Date date(stoi(fieldVal.substr(0,2)), stoi(fieldVal.substr(2,2)), stoi(fieldVal.substr(4)));
                            currentTask->setEndDate(date);
                        }
                    }
                }
            currentFile.close();
            }
        }
    list.close();
}
void TaskManager::saveTasks()
{
    std::ofstream list("tasks/list.ctdfs"), out;
    for(Folder* folder : Folders)
    {
        if (!fs::exists("tasks")) if (fs::create_directories("tasks")) std::cout << "NOTE: Created \"tasks/\" for saving tasks." << std::endl;
        out.open("tasks/"+folder->getName()+".ctdf");
        list << "tasks/"+folder->getName()+".ctdf\n";
        out << "folder=" << folder->getName();
        if(folder->getTasks().size()!=0) out << "\nnext\n";
        for(Task* task : folder->getTasks())
        {
            out << "name=" << task->getName() << "\n";
            out << "status=" << (int)(task->getStatus()) << "\n";
            out << "sdate=";
            for(int i=0; i<2-std::to_string(task->getStartDate()->getDay()).length(); i++) out << "0";
            out << task->getStartDate()->getDay();
            for(int i=0; i<2-std::to_string(task->getStartDate()->getMonth()).length(); i++) out << "0";
            out << task->getStartDate()->getMonth();
            for(int i=0; i<4-std::to_string(task->getStartDate()->getYear()).length(); i++) out << "0";
            out << task->getStartDate()->getYear() << "\n";
            out << "edate=";
            for(int i=0; i<2-std::to_string(task->getEndDate()->getDay()).length(); i++) out << "0";
            out << task->getEndDate()->getDay();
            for(int i=0; i<2-std::to_string(task->getEndDate()->getMonth()).length(); i++) out << "0";
            out << task->getEndDate()->getMonth();
            for(int i=0; i<4-std::to_string(task->getEndDate()->getYear()).length(); i++) out << "0";
            out << task->getEndDate()->getYear();
            if(task != folder->getTasks().back()) out << "\nnext\n";
        }
        out.close();
    }
}
Task* TaskManager::getTaskByName(const std::string& taskname) const
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
Folder* TaskManager::getFolderByName(const std::string& foldername) const
{
    for(Folder* folder : Folders)
    {
        if(folder->getName() == foldername) return folder;
    }
    throw std::invalid_argument("task \""+foldername+"\" not found");
}
Folder* TaskManager::getFolderByTask(Task*& targetTask) const
{
    for(Folder* folder : Folders)
    {
        for(Task* task : folder->getTasks())
        {
            if(task->getID() == targetTask->getID()) return folder;
        }
    }
    throw std::invalid_argument("task not found");
}
std::vector<Task*> TaskManager::getAllTasks() const
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
std::vector<Task*> TaskManager::getTasksByDate(const int& day, const int& month, const int& year) const
{
    std::vector<Task*> ret;
    for(Folder* folder : Folders)
    {
        for(Task* task : folder->getTasks())
        {
            if(task->getEndDate()->getDay()==day && task->getEndDate()->getMonth()==month && task->getEndDate()->getYear()==year) ret.push_back(task);
        }
    }
    return ret;
}
std::vector<Task*> TaskManager::getTasksFromFolder(const std::string& folderName) const
{
    for(Folder* folder : Folders)
    {
        if(folder->getName()==folderName) return folder->getTasks();
    }
    throw std::invalid_argument("folder \""+folderName+"\" not found");
}
std::vector<Task*> TaskManager::getTasksFromFolder(Folder* targetFolder) const
{
    return targetFolder->getTasks();
    throw std::invalid_argument("folder not found");
}
void TaskManager::printDelimeter(const int& nameLength, const int& mode) const
{
    std::array<std::string, 3> s;
    switch(mode)
    {
        case 0: s = {"┏","┳","┓"}; break;
        case 1: s = {"┣","╋","┫"}; break;
        case 2: s = {"┗","┻","┛"}; break;
        default: throw std::invalid_argument("printDelimeter: mode should be 0, 1 or 2, not "+std::to_string(mode));
    }
    std::cout << s[0];
    for(int i=0; i<nameLength+2; ++i) std::cout << "━";
    std::cout << s[1];
    for(int i=0; i<8; ++i) std::cout << "━";
    std::cout << s[1];
    for(int i=0; i<20; ++i) std::cout << "━";
    std::cout << s[1];
    for(int i=0; i<20; ++i) std::cout << "━";
    std::cout << s[2] << std::endl;
}
void TaskManager::printAllTasks() const
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
    printDelimeter(nameLength, 0);
    //Upper bar output, format: | Name | Status | Due | Created |
    std::cout << "┃ Name ";
    for(int i=0; i<nameLength-4; ++i) std::cout << " ";
    std::cout << "┃ Status ┃ Due ";
    for(int i=0; i<15; ++i) std::cout << " ";
    std::cout << "┃ Created ";
    for(int i=0; i<11; ++i) std::cout << " ";
    std::cout << "┃" << std::endl;

    for(Folder* folder : Folders)
    {
        printDelimeter(nameLength, 1);

        //Folder header output
        std::cout << "\033[30;47m " << folder->getName();
        for(int i=0; i<nameLength+54-(folder->getName().length()); ++i) std::cout << " ";
        std::cout << "\033[0m" << std::endl;

        for(Task* task : folder->getTasks())
        {
            printDelimeter(nameLength, 1);
            // if task is done, it'll be printed gray, if undone - standart (white)
            if(task->getStatus()) std::cout << "\033[90m";
            // name output
            std::cout << "┃ " << task->getName();
            for(int i=0; i<nameLength-task->getName().length()+1; ++i) std::cout << " ";
            // status output
            std::cout << "┃ ";
            if(task->getStatus()) std::cout << "Done   ";
            else std::cout << "Undone ";
            // deadline (endDate) output
            std::cout << "┃ " << task->getEndDate()->print();
            for(int i=0; i<19-(task->getEndDate()->print().length()); ++i) std::cout << " ";
            // creation date (startDate) output
            std::cout << "┃ " << task->getStartDate()->print();
            for(int i=0; i<19-(task->getStartDate()->print().length()); ++i) std::cout << " ";
            // final "┃" output
            std::cout << "┃ \033[0m" << std::endl;
        }
    }
    printDelimeter(nameLength, 2);
}
void TaskManager::printTasks(const std::vector<Task*>& tasks) const
{
    int nameLength = 4;
    for(Task* task : tasks)
    {
        if(task->getName().length() > nameLength) nameLength = task->getName().length();
    }
    printDelimeter(nameLength, 0);
    //Upper bar output, format: | Name | Status | Due | Created |
    std::cout << "┃ Name ";
    for(int i=0; i<nameLength-4; ++i) std::cout << " ";
    std::cout << "┃ Status ┃ Due ";
    for(int i=0; i<15; ++i) std::cout << " ";
    std::cout << "┃ Created ";
    for(int i=0; i<11; ++i) std::cout << " ";
    std::cout << "┃" << std::endl;

    for(Task* task : tasks)
    {
        printDelimeter(nameLength, 1);
        // name output
        std::cout << "┃ " << task->getName();
        for(int i=0; i<nameLength-task->getName().length()+1; ++i) std::cout << " ";
        // status output
        std::cout << "┃ ";
        if(task->getStatus()) std::cout << "Done   ";
        else std::cout << "Undone ";
        // deadline (endDate) output
        std::cout << "┃ " << task->getEndDate()->print();
        for(int i=0; i<19-(task->getEndDate()->print().length()); ++i) std::cout << " ";
        // creation date (startDate) output
        std::cout << "┃ " << task->getStartDate()->print();
        for(int i=0; i<19-(task->getStartDate()->print().length()); ++i) std::cout << " ";
        // final "┃" output
        std::cout << "┃ " << std::endl;
    }
    printDelimeter(nameLength, 2);
}
void TaskManager::printLogo() const
{
    std::cout << ctdinfo.logo << "\n";
}