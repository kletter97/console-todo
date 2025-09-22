#include "Task.hpp"
#include <iostream>

Task::Task()
{
    name = "testTask";
    setStatus(false);
    //setStartDate(Date());
    //setEndDate(Date());
}
Task::Task(const std::string inName, const Date& inEndDate)
{
    setName(inName);
    setStartDate(Date());
    setEndDate(inEndDate);
    setStatus(false);
}
std::string Task::getName() const
{
    return name;
}
Date Task::getStartDate() const
{
    return startDate;
}
Date Task::getEndDate() const
{
    return endDate;
}
bool Task::getStatus() const
{
    return isCompleted;
}
void Task::setName(const std::string newName)
{
    name = newName;
}
void Task::setStartDate(const Date& newDate)
{
    startDate = newDate;
}
void Task::setEndDate(const Date& newDate)
{
    endDate = newDate;
}
void Task::setStatus(const bool newStatus)
{
    isCompleted = newStatus;
}