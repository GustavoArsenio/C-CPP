#ifndef CALENDAR_DATE_H
#define CALENDAR_DATE_H

#include <stdbool.h>

enum month {
    MONTH_JANUARY = 1,
    MONTH_FEBRUARY,
    MONTH_MARCH,
    MONTH_APRIL,
    MONTH_MAY,
    MONTH_JUNE,
    MONTH_JULY,
    MONTH_AUGUST,
    MONTH_SEPTEMBER,
    MONTH_OCTOBER,
    MONTH_NOVEMBER,
    MONTH_DECEMBER
};

struct date {
    int year;
    enum month month;
    int day;
};

/* A valid date has a positive year and a day in its month's range. */
bool date_is_valid(struct date value);

#endif
