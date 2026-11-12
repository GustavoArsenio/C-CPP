#include <assert.h>

#include "calendar_date.h"

int main(void) {
    assert(date_is_valid((struct date){2024, MONTH_FEBRUARY, 29}));
    assert(!date_is_valid((struct date){1900, MONTH_FEBRUARY, 29}));
    assert(date_is_valid((struct date){2000, MONTH_FEBRUARY, 29}));
    assert(date_is_valid((struct date){2025, MONTH_APRIL, 30}));
    assert(!date_is_valid((struct date){2025, MONTH_APRIL, 31}));
    assert(!date_is_valid((struct date){2025, MONTH_JANUARY, 0}));
    assert(!date_is_valid((struct date){0, MONTH_JANUARY, 1}));
    assert(!date_is_valid((struct date){2025, (enum month)0, 1}));
    assert(!date_is_valid((struct date){2025, (enum month)13, 1}));
    return 0;
}
