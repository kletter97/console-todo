#include "Task.hpp"

Task::Task()
{
    setName("Unnamed Task");
    setStartDate(Date());
    setEndDate(Date());
    setStatus(false);
}
Task::Task(const std::string inName, const Date& inEndName)
{
    setName(inName);
    setStartDate(Date());
    setEndDate(inEndName);
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
bool Task::setStatus(const bool newStatus)
{
    isCompleted = newStatus;
}