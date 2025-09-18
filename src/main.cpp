#include <iostream>
#include "Task.hpp"

int main()
{
    Date date((unsigned)29, (unsigned)2, 2024);
    std::cout << "Date: " << date.print() << std::endl;
    return 0;
}