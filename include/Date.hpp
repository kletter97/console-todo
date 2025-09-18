#ifndef DATE_H
#define DATE_H

#include <string>
#include <array>

class Date
{
    private:
        unsigned day;
        unsigned month;
        int year;
        std::array<unsigned, 12> daysInMonths;
        std::array<std::string, 12> monthsNames;
    public:
        Date();
        Date(const unsigned inDay, const unsigned inMonth, const int inYear);
        unsigned getDay() const;
        unsigned getMonth() const;
        int getYear() const;
        void setDay(const unsigned newDay);
        void setMonth(const unsigned newMonth);
        void setYear(const int newYear);
        void move(const int days);
        std::string print();
};

#endif //DATE_H