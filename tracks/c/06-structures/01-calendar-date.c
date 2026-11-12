/* A struct groups date fields; the enum gives months meaningful names. */

#include <stdio.h>

#include "calendar_date.h"

int main(void) {
    const struct date examples[] = {
        {.year = 2024, .month = MONTH_FEBRUARY, .day = 29},
        {.year = 1900, .month = MONTH_FEBRUARY, .day = 29},
        {.year = 2000, .month = MONTH_FEBRUARY, .day = 29},
        {.year = 2025, .month = MONTH_APRIL, .day = 31}};
    const char *const labels[] = {"2024-02-29", "1900-02-29", "2000-02-29",
                                  "2025-04-31"};

    for (size_t index = 0; index < sizeof(examples) / sizeof(examples[0]);
         ++index) {
        printf("%s: %s\n", labels[index],
               date_is_valid(examples[index]) ? "valid" : "invalid");
    }
    return 0;
}
