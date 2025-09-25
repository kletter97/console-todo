#include <iostream>
#include <fstream>
#include "TaskManager.hpp"

int main()
{
    
    Date date((unsigned)29, (unsigned)2, 2027), curDate;
    Task task("Example Task", date);
    TaskManager TM;
    std::cout << "\033[?1049h";
    while(true)
    {
        TM.printInterface();
    }
    std::cout << "\033[?1049l";
    return 0;
}