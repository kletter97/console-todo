#include "Date.hpp"
#include <chrono>
#include <vector>
#include <string>
#include <iostream>

Date::Date()
{
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    std::chrono::year_month_day ymd = std::chrono::year_month_day{today};
    setYear((int)ymd.year());
    setMonth((unsigned)ymd.month()-1);
    setDay((unsigned)ymd.day());
    monthsNames = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
}
Date::Date(const unsigned inDay, const unsigned inMonth, const int inYear)
{
    setYear(inYear);
    setMonth(inMonth);
    setDay(inDay);
    monthsNames = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
}
unsigned Date::getDay() const
{
    return day;
}
unsigned Date::getMonth() const
{
    return month;
}
int Date::getYear() const
{
    return year;
}
void Date::setDay(const unsigned newDay)
{
    if(newDay==0) throw std::invalid_argument("day cannot be zero");
    else if(newDay > daysInMonths[month])
        throw std::invalid_argument(std::to_string(newDay)+" is invalid day for month "+std::to_string(month)+" with "+std::to_string(daysInMonths[month])+" days");
    day = newDay;
}
void Date::setMonth(const unsigned newMonth)
{
    if(newMonth<0 || newMonth>11) throw std::invalid_argument("month cannot be negative or bigger than 11");
    if(day > daysInMonths[newMonth]) day = 1;
    month = newMonth;
}
void Date::setYear(const int newYear)
{
    if(newYear<0) throw std::invalid_argument("year cannot be negative");
    year = newYear;
    if(newYear%4==0 && newYear%400!=0) daysInMonths = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    else daysInMonths = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
}
void Date::move(int days)
{
    if(day+days>0 && day+days<=daysInMonths[month]) day += days; return;
    if(day+days>daysInMonths[month])
    {
        int diff = day+days-daysInMonths[month];
        if(month!=11) {month++; day = diff; return;}
        else {setYear(year+1); month = 0; day = diff; return;}
    }
    if(day+days<=0)
    {
        int diff = day+days;
        if(month!=0) {month--; day = daysInMonths[month]-diff; return;}
        else {setYear(year-1); month = 11; day = daysInMonths[month]-diff; return;}
    }
    return;
}
std::string Date::print()
{
    return monthsNames[month]+" "+std::to_string(day)+", "+std::to_string(year);
}