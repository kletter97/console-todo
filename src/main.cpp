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
    Task task("Example Task", date);
    TaskManager testTM;
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