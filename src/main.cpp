#include <iostream>
#include "Task.hpp"

int main()
{
    Date date((unsigned)29, (unsigned)2, 2027), curDate;
    std::cout << "Date: " << curDate.print() << std::endl;
    Task task("Example Task", date);
    std::cout << "Task: " << task.getName() << ", End Date: " << task.getEndDate().print() << std::endl;
    return 0;
}