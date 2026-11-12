#include "calendar_date.h"

static bool is_leap_year(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

bool date_is_valid(struct date value) {
    if (value.year < 1 || value.month < MONTH_JANUARY ||
        value.month > MONTH_DECEMBER || value.day < 1) {
        return false;
    }

    static const int days_per_month[] = {31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31};
    int limit = days_per_month[(int)value.month - 1];
    if (value.month == MONTH_FEBRUARY && is_leap_year(value.year)) {
        limit = 29;
    }
    return value.day <= limit;
}
