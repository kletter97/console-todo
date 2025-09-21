#include <iostream>
#include "TaskManager.hpp"

int main()
{
    std::string command;
    Date date((unsigned)29, (unsigned)2, 2027), curDate;
    //std::cout << "Date: " << curDate.print() << std::endl;
    Task task("Example Task", date);
    //std::cout << "Task: " << task.getName() << ", End Date: " << task.getEndDate().print().length() << std::endl;
    TaskManager testTM(true);
    testTM.printAllTasks();
    //puts("this should be before error");
    //testTM.getTaskByName("kirogaz")->getEndDate().print();
    while(true)
    {
        std::cout << "console-todo> ";
        getline(std::cin, command);
        testTM.parseInput(command);
    }
    return 0;
}