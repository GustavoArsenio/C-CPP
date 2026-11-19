#include <stdbool.h>
#include <stdio.h>

enum { MAX_POINTS = 1000 };

int main(void) {
    int point_count;
    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xx = 0.0;
    double sum_xy = 0.0;

    if (scanf("%d", &point_count) != 1 || point_count < 2 ||
        point_count > MAX_POINTS) {
        fprintf(stderr, "Point count must be from 2 through %d.\n", MAX_POINTS);
        return 1;
    }

    for (int index = 0; index < point_count; ++index) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            fprintf(stderr, "Expected one x/y pair per point.\n");
            return 1;
        }
        sum_x += x;
        sum_y += y;
        sum_xx += x * x;
        sum_xy += x * y;
    }

    double denominator = point_count * sum_xx - sum_x * sum_x;
    if (denominator == 0.0) {
        fprintf(stderr,
                "Regression is undefined when all x values are equal.\n");
        return 1;
    }

    double slope = (point_count * sum_xy - sum_x * sum_y) / denominator;
    double intercept = (sum_y - slope * sum_x) / point_count;
    printf("G(x) = %.6f + %.6f x\n", intercept, slope);
    return 0;
}
