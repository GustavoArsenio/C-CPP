#ifndef INT_MIN_MAX_H
#define INT_MIN_MAX_H

/* Find the minimum and maximum of four values.
 * Returns 1 on success and 0 if an output pointer is null or both output
 * pointers are identical. Outputs are only written on success.
 */
int int_min_max_four(int first, int second, int third, int fourth, int *minimum,
                     int *maximum);

#endif
