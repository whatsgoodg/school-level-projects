#include <stdio.h>
#include "coordi.h"
// print manhattan distance
void print_distance(const struct coordinate* x, const struct coordinate* y) {
        int row = x->longitude.totalSec - y->longitude.totalSec;
        int col = x->latitude.totalSec - y->latitude.totalSec;
        if (row < 0) row = -1 * row;
        if (col < 0) col = -1 * col;
        int dis = row + col;
        int tmp = dis;
        int degree = tmp / 3600;
        tmp = tmp % 3600;
        int minute = tmp / 60;
        tmp = tmp % 60;
        printf(" %06d %03dd %02dm %02ds\n", dis, degree, minute, tmp);
}
