#include "int_min_max.h"

int int_min_max_four(int first, int second, int third, int fourth, int *minimum,
                     int *maximum) {
    int smallest;
    int largest;

    if (minimum == 0 || maximum == 0 || minimum == maximum) {
        return 0;
    }

    smallest = first;
    largest = first;
    if (second < smallest) {
        smallest = second;
    }
    if (second > largest) {
        largest = second;
    }
    if (third < smallest) {
        smallest = third;
    }
    if (third > largest) {
        largest = third;
    }
    if (fourth < smallest) {
        smallest = fourth;
    }
    if (fourth > largest) {
        largest = fourth;
    }

    *minimum = smallest;
    *maximum = largest;
    return 1;
}
