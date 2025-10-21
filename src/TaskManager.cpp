#include "TaskManager.hpp"
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>
#include <algorithm>
#include "getch.hpp"

namespace fs = std::filesystem;

TaskManager::TaskManager()
{
    displayMode = true;     //true for all tasks printing on start
    allOrUndoneMode = true; //true for all tasks printing on start
    currentNote = "";
    try {readTasks();}
    catch(...) {std::cout << "ERROR: Tasks files are corrupted, reading failed. Program has been stopped.\n"; std::exit(1);}
    if(Folders.size()==0)
    {
        currentNote = COLOR_TEXT_NOTE + (std::string)"NOTE: No tasks found, created an empty folder \"General\".\n" + RESET_COLOR;
        Folder* General = new Folder("General");
        Folders.push_back(General);
    }
}
TaskManager::TaskManager(bool mode)
{
    // just for testing the class without import/export tasks (those methods are not created so far)
    displayMode = true; //true for all tasks printing on start
    currentNote = "";
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
        if(words[1]=="all") displayMode=true;
        else if(words[1]=="date" || words[1]=="folder")
        {
            displayMode = false;
            //if(words[1]=="date") tasksForDisplay = getTasksByDate(stoi(words[2]), stoi(words[3])-1, stoi(words[4]));
            //else
            if(words[1]=="folder") folderForDisplay = getFolderByName(words[2]);
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
                Date newDate((unsigned)stoi(words[3]), (unsigned)stoi(words[4])-1, stoi(words[5]));
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
            Date* newDate = new Date((unsigned)stoi(words[4]), (unsigned)stoi(words[5])-1, stoi(words[6]));
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
            Task* targetTask = getTaskByName(words[2]);
            Folder* targetFolder = getFolderByTask(targetTask);
            targetFolder->removeTask(targetTask);
            delete targetTask;
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
    else if(words[0]=="move")
    {
        Task* targetTask = getTaskByName(words[1]);
        Folder* targetFolder = getFolderByName(words[2]);
        moveTaskToFolder(targetTask, targetFolder);
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
    else if(words[0]=="about") currentNote = COLOR_TEXT_HELP + ctdinfo.getInfoText() + "\n" + RESET_COLOR;
    else if(words[0]=="help") currentNote = COLOR_TEXT_HELP + (std::string)ctdinfo.getHelp(words[1]) + "\n" + RESET_COLOR;
    else if(words[0]=="exit")
    {
        std::cout << "saving your tasks..." << "\n";
        saveTasks();
        std::cout << "\033[?1049l";
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
        out << "folder=" << folder->getName() << "\n";
        if(folder->getTasks().size()!=0) out << "next\n";
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
void TaskManager::moveTaskToFolder(Task* targetTask, Folder* newFolder)
{
    Folder* oldFolder = getFolderByTask(targetTask);
    newFolder->addTask(targetTask);
    oldFolder->removeTask(targetTask);
}

std::string TaskManager::formDelimeter(const int& nameLength, const int& mode) const
{
    std::string ret;
    std::array<std::string, 3> s;
    switch(mode)
    {
        case 0: s = {"┏","┳","┓"}; break;
        case 1: s = {"┣","╋","┫"}; break;
        case 2: s = {"┗","┻","┛"}; break;
        default: throw std::invalid_argument("printDelimeter: mode should be 0, 1 or 2, not "+std::to_string(mode));
    }
    ret += s[0];
    for(int i=0; i<nameLength+2; ++i) ret += "━";
    ret += s[1];
    for(int i=0; i<8; ++i) ret += "━";
    ret += s[1];
    for(int i=0; i<DATE_MAX_LENGTH+1; ++i) ret += "━";
    ret += s[1];
    for(int i=0; i<DATE_MAX_LENGTH+1; ++i) ret += "━";
    ret += s[2];
    return ret;
}

std::vector<std::string> TaskManager::formTasksTable(const Folder* openedFolder, const unsigned sideBarHeight) const
{
    std::vector<std::string> ret;
    int nameLength = 4;

    std::vector<Task*> tasks;
    if(allOrUndoneMode) tasks = openedFolder->getTasks();
    else tasks = openedFolder->getUndoneTasks();

    Date todayDate;
    
    for(Task* task : tasks)
    {
        if(task->getName().length() > nameLength) nameLength = task->getName().length();
    }
    //Upper line creating
    ret.push_back(formDelimeter(nameLength, 0));

    std::string currentLine="";
    //Upper bar creating, format: | Name | Status | Due | Created |
    currentLine += "┃ Name ";
    for(int i=0; i<nameLength-4; ++i) currentLine += " ";
    currentLine += "┃ Status ┃ Due ";
    for(int i=0; i<DATE_MAX_LENGTH-4; ++i) currentLine += " ";
    currentLine += "┃ Created ";
    for(int i=0; i<DATE_MAX_LENGTH-8; ++i) currentLine += " ";
    currentLine += "┃";
    ret.push_back(currentLine);

    //Tasks lines creating
    for(Task* task : tasks)
    {
        currentLine="";
        // middle delimeter creating
        ret.push_back(formDelimeter(nameLength, 1));
        // if task is done, it'll be printed gray, if undone - standart (white)
        if(task->getStatus()) currentLine += COLOR_TEXT_DONE;
        else if(todayDate > *(task->getEndDate())) currentLine += COLOR_TEXT_OVERDUE;
        // name output
        currentLine += "┃ " + task->getName();
        for(int j=0; j<nameLength-task->getName().length()+1; j++) currentLine += " ";
        // status output
        currentLine += "┃ ";
        if(task->getStatus()) currentLine += "Done   ";
        else currentLine += "Undone ";
        // deadline (endDate) output
        currentLine += "┃ " + task->getEndDate()->print();
        for(int j=0; j<DATE_MAX_LENGTH-(task->getEndDate()->print().length()); j++) currentLine += " ";
        // creation date (startDate) output
        currentLine += "┃ " + task->getStartDate()->print();
        for(int j=0; j<DATE_MAX_LENGTH-(task->getStartDate()->print().length()); j++) currentLine += " ";
        // final "┃" output
        currentLine += "┃" + (std::string)RESET_COLOR;
        ret.push_back(currentLine);
    }

    // Filling with empty lines if sidebar is higher than tasks table
    while(ret.size()<sideBarHeight+SIDE_BAR_VERTICAL_INDENT)
    {
        currentLine = "";
        // middle delimeter creating
        ret.push_back(formDelimeter(nameLength, 1));
        // empty table line
        currentLine += "┃";
        for(int i=0; i<nameLength+2; ++i) currentLine += " ";
        currentLine += "┃        ┃";
        for(int i=0; i<DATE_MAX_LENGTH+1; ++i) currentLine += " ";
        currentLine += "┃";
        for(int i=0; i<DATE_MAX_LENGTH+1; ++i) currentLine += " ";
        currentLine += "┃";
        ret.push_back(currentLine);
    }

    //Bottom line creating
    ret.push_back(formDelimeter(nameLength, 2));
    return ret;
}

void TaskManager::printTasks(const Folder* outputFolder) const
{
    unsigned indent;
    std::vector<std::string> sideBar = formFolderSideBar(indent, outputFolder);
    std::vector<std::string> tasksTable = formTasksTable(outputFolder, sideBar.size());

    for(int i=0; i<std::max<int>(sideBar.size(), tasksTable.size()); i++)
    {
        if(i<sideBar.size()+SIDE_BAR_VERTICAL_INDENT && i>=SIDE_BAR_VERTICAL_INDENT) std::cout << sideBar[i-SIDE_BAR_VERTICAL_INDENT] << tasksTable[i] << std::endl;
        else 
        {
            for(int j=0; j<indent; j++) std::cout << " ";
            std::cout << tasksTable[i] << std::endl;
        }
    }
}
void TaskManager::printLogo() const
{
    std::cout << ctdinfo.logo << "\n";
}
void TaskManager::clearScreen() const
{
    std::cout << "\033[2J\033[H";
}
std::array<int, 2> TaskManager::getTerminalSize() const
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return {w.ws_row, w.ws_col};
}
std::array<int, 2> TaskManager::getIndents(int xLen, int yLen) const
{
    std::array<int, 2> terSize = getTerminalSize();
    return {0, 0};
    //return {(terSize[1]-xLen)/2, (terSize[0]-yLen)/2};
}
std::vector<std::string> TaskManager::formFolderSideBar(unsigned& indentLength, const Folder* openedFolder) const // indentLength is an empty variable to get indent for main table
{
    std::vector<std::string> ret;
    std::string currentStr, color, allColor, undoneColor;
    unsigned length = 10;
    for(Folder* folder : Folders) if(folder->getName().length() > length) length = folder->getName().length();
    indentLength = length+1;

    // "All tasks" tab
    if(!displayMode)
    {
        color = COLOR_TEXT_DONE;                         //grey if not selected
    }
    else color = "";

    if(!allOrUndoneMode) 
    {
        allColor = COLOR_TEXT_DONE;                  //grey if not selected "All" (allOrUndoneMode = false)
        undoneColor = RESET_COLOR;                //white if selected "Undone"
    }
    else
    {
        allColor = RESET_COLOR;                  //white if selected "All" (allOrUndoneMode = true)
        undoneColor = COLOR_TEXT_DONE;              //grey if not selected "Undone"
    }
    
    // upper border
    currentStr = color + "┏";
    for(int i=0; i<length; i++) currentStr += "━";
    currentStr += RESET_COLOR;
    ret.push_back(currentStr);
    // body
    
    currentStr = color + "┃" + allColor + "All" + COLOR_TEXT_DONE + "/" + undoneColor + "Undone" + RESET_COLOR;
    for(int i=0; i<length-10; i++) currentStr += " ";
    ret.push_back(currentStr);
    // bottom border
    currentStr = color + "┗";
    for(int i=0; i<length; i++) currentStr += "━";
    currentStr += RESET_COLOR;
    ret.push_back(currentStr);

    // folder tabs
    for(Folder* folder : Folders)
    {
        if(openedFolder->getName()!=folder->getName()) color = COLOR_TEXT_DONE; //grey if not selected
        else color = "";
        // upper border
        currentStr = color + "┏";
        for(int i=0; i<length; i++) currentStr += "━";
        currentStr += RESET_COLOR;
        ret.push_back(currentStr);

        // body
        currentStr = color + "┃" + folder->getName() + RESET_COLOR;
        for(int i=0; i<length-(folder->getName().length()); i++) currentStr += " ";
        ret.push_back(currentStr);

        // bottom border
        currentStr = color + "┗";
        for(int i=0; i<length; i++) currentStr += "━";
        currentStr += RESET_COLOR;
        ret.push_back(currentStr);
    }
    return ret;
}
Folder TaskManager::allTasksFolder() const
{
    // create folder with all tasks
    Folder allTasks("serviceAllTasksFolder");
    for(Folder* folder : Folders) for (Task* task : folder->getTasks())
    {
        allTasks.addTask(task);
    }
    return allTasks;
}
void TaskManager::nextOrPrevFolder(bool mode)
{
    auto it = std::find(Folders.begin(), Folders.end(), folderForDisplay);
    if(mode) // next folder
    {
        if(displayMode) {folderForDisplay = Folders[0]; displayMode = false;}
        else if(it != Folders.end() && it + 1 != Folders.end()) folderForDisplay = *(it + 1);
    }
    else // previous folder
    {
        if(folderForDisplay == Folders[0]) displayMode = true;
        else if(it != Folders.begin()) folderForDisplay = *(it - 1);
    }
}
void TaskManager::printInterface()
{
    std::string command;
    clearScreen();
    printLogo();
    if(displayMode)
    {
        Folder allTasks = allTasksFolder();
        Folder* allTasksPointer = &allTasks;
        printTasks(allTasksPointer);
    }
    else printTasks(folderForDisplay);
    std::cout << currentNote << "console-todo> ";

    // here's a custom input catcher, because program needs to catch arrow keys without clicking "enter", so it reads symbols by getch() (getch.hpp) and analyze them
    while(true)
    {    
        int k = getch(); // getch() reads a single character without waiting for Enter, watch getch.hpp
        if(k == 27) // if the first character is ESC (checking for arrows)
        {
            k = getch();
            if(k == 91) // and the second character is [
            {
                k = getch();
                switch(k) // the third character specifies the arrow key
                {
                    case 65: nextOrPrevFolder(0); break; // Up arrow
                    case 66: nextOrPrevFolder(1); break; // Down arrow
                    case 67: allOrUndoneMode = !allOrUndoneMode; break; // Right arrow
                    case 68: allOrUndoneMode = !allOrUndoneMode; break; // Left arrow
                    default: break; // if not an arrow, return the character to input stream
                }
            }
            //else ungetc(k, stdin); // if not [, return the character to input stream
            return;
        }
        else if(k == 127 || k == 8) // backspace
        {
            if(!command.empty())
            {
                command.pop_back();
                std::cout << "\b \b" << std::flush; // output backspace so last symbol deleted in terminal
            }
        }
        else if(k == 13 || k == 10) // enter
        {
            try {currentNote = ""; parseInput(command);}
            catch (const std::invalid_argument& e) {currentNote = COLOR_TEXT_ERROR + (std::string)"ERROR: " + (std::string)e.what() + "\n" + RESET_COLOR;}
            return;
        }
        else // regular symbol, add to command and show in terminal
        {
            command.push_back((char)k);
            std::cout << (char)k;
        }
    }
}