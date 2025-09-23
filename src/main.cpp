#include <iostream>
#include <fstream>
#include "TaskManager.hpp"

void printLogo()
{
    std::ifstream in("resources/logo.txt");;
    std::string output;
    if (in.is_open())
    {
        while (std::getline(in, output))
        {
            std::cout << output << std::endl;
        }
    }
    in.close();
}

int main()
{
    
    std::string command;
    printLogo();
    Date date((unsigned)29, (unsigned)2, 2027), curDate;
    //std::cout << "Date: " << curDate.print() << std::endl;
    Task task("Example Task", date);
    //std::cout << "Task: " << task.getName() << ", End Date: " << task.getEndDate().print().length() << std::endl;
    TaskManager testTM;
    testTM.printAllTasks();
    //puts("this should be before error");
    //testTM.getTaskByName("kirogaz")->getEndDate().print();
    while(true)
    {
        std::cout << "console-todo> ";
        getline(std::cin, command);
        testTM.parseInput(command);
        testTM.saveTasks();
    }
    return 0;
}