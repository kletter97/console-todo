#include <iostream>
#include <fstream>
#include "TaskManager.hpp"

int main()
{
    
    std::string command ;
    Date date((unsigned)29, (unsigned)2, 2027), curDate;
    Task task("Example Task", date);
    TaskManager testTM;
    testTM.printLogo();
    testTM.printAllTasks();
    while(true)
    {
        std::cout << "console-todo> ";
        getline(std::cin, command);
        try {testTM.parseInput(command);}
        catch (const std::invalid_argument& e) {std::cout << "ERROR: " << e.what() << "\n";}
    }
    return 0;
}