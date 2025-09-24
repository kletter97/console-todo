#include "Task.hpp"
#include <iostream>

int Task::idCounter = 0;

Task::Task()
{
    id = idCounter++;
    name = "testTask";
    setStatus(false);
}
Task::Task(const std::string inName, const Date& inEndDate)
{
    id = idCounter++;
    setName(inName);
    setStartDate(Date());
    setEndDate(inEndDate);
    setStatus(false);
}
std::string Task::getName() const
{
    return name;
}
int Task::getID() const
{
    return id;
}
Date* Task::getStartDate()
{
    return &startDate;
}
Date* Task::getEndDate()
{
    return &endDate;
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
Task::~Task() {}