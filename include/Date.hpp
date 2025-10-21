#ifndef DATE_H
#define DATE_H

#include <string>
#include <array>

class Date
{
    private:
        unsigned weekday;
        unsigned day;
        unsigned month;
        int year;
        std::array<unsigned, 12> daysInMonths;
        std::array<std::string, 12> monthsNames;
        std::array<std::string, 7> weekdaysNames;
    public:
        Date();
        Date(const unsigned inDay, const unsigned inMonth, const int inYear);
        unsigned getWeekday() const;
        unsigned getDay() const;
        unsigned getMonth() const;
        int getYear() const;
        void setWeekday(const unsigned newWeekday);
        void setDay(const unsigned newDay);
        void setMonth(const unsigned newMonth);
        void setYear(const int newYear);
        void move(const int days);
        std::string print();

        bool operator< (Date another);
        bool operator> (Date another);
};

#endif //DATE_H