#ifndef TASK_H
#define TASK_H

#include <string>
#include "Date.hpp"

class Task
{
    private:
        static int idCounter;
        int id;
        std::string name;
        Date startDate, endDate;
        bool isCompleted;
    public:
        Task();
        Task(const std::string inName, const Date& inEndDate);
        std::string getName() const;
        int getID() const;
        Date* getStartDate();
        Date* getEndDate();
        bool getStatus() const;
        void setName(const std::string newName);
        void setStartDate(const Date& newDate);
        void setEndDate(const Date& newDate);
        void setStatus(const bool newStatus);
        ~Task();
};

#endif //TASK_H