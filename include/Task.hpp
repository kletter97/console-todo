#ifndef TASK_H
#define TASK_H

#include <string>
#include "Date.hpp"

class Task
{
    private:
        std::string name;
        Date startDate, endDate;
        bool isCompleted;
    public:
        Task();
        Task(const std::string inName, const Date& inEndDate);
        std::string getName() const;
        Date getStartDate() const;
        Date getEndDate() const;
        bool getStatus() const;
        void setName(const std::string newName);
        void setStartDate(const Date& newDate);
        void setEndDate(const Date& newDate);
        void setStatus(const bool newStatus);
};

#endif //TASK_H