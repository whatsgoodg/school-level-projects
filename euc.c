#include <stdio.h>
#include <math.h>
#include "coordi.h"
// get euclidean distance
void print_distance(const struct coordinate* x, const struct coordinate* y) {
        int row = x->longitude.totalSec - y->longitude.totalSec;
        int col = x->latitude.totalSec - y->latitude.totalSec;
        int dis = (int)sqrt(row * row + col * col);
        int tmp = dis;
        int degree = tmp / 3600;
        tmp = tmp % 3600;
        int minute = tmp / 60;
        tmp = tmp % 60;
        printf(" %06d %03dd %02dm %02ds\n", dis, degree, minute, tmp);
}
