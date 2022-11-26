#include <stdio.h>
#include <stdlib.h>
#include "coordi.h"
// get data from text file
int getGIS(char* filename, struct coordinate* a, struct coordinate* b) {
        printf("%s\n", filename);
        FILE* fp = fopen(filename, "r");
        int cur = 0;
        while (!feof(fp)) {
                fscanf(fp, "(%d%c, %d%c) (%d%c, %d%c) \n",
                        &a[cur].longitude.dms, &a[cur].longitude.direction,
                        &a[cur].latitude.dms, &a[cur].latitude.direction,
                        &b[cur].longitude.dms, &b[cur].longitude.direction,
                        &b[cur].latitude.dms, &b[cur].latitude.direction);

                cur++;
        }
        fclose(fp);
        return cur;
}
// get degree, minute, second, totalSec
void setInfos(struct coordinate* a, struct coordinate* b) {
        int cnt = 2;
        int longiA = a->longitude.dms;
        int latiA = a->latitude.dms;
        int longiB = b->longitude.dms;
        int latiB = b->latitude.dms;
        while (cnt--) {
                if (cnt == 1) {
                        a->longitude.second = longiA % 100;
                        a->latitude.second = latiA % 100;
                        b->longitude.second = longiB % 100;
                        b->latitude.second = latiB % 100;
                }
                else {
                        a->longitude.minute = longiA % 100;
                        a->latitude.minute = latiA % 100;
                        b->longitude.minute = longiB % 100;
                        b->latitude.minute = latiB % 100;
                }
                latiA /= 100;
                longiA /= 100;
                latiB /= 100;
                longiB /= 100;
        }
        a->latitude.degree = latiA;
        a->longitude.degree = longiA;
        b->latitude.degree = latiB;
        b->longitude.degree = longiB;
        // get total sec
        a->latitude.totalSec = 0;
	a->longitude.totalSec = 0;
        b->latitude.totalSec = 0;
        b->longitude.totalSec = 0;
        cnt = 3;
        while (cnt--) {
                if (cnt == 2) {
                        a->latitude.totalSec += a->latitude.degree * 3600;
                        a->longitude.totalSec += a->longitude.degree * 3600;
                        b->latitude.totalSec += b->latitude.degree * 3600;
                        b->longitude.totalSec += b->longitude.degree * 3600;
                }
                else if (cnt == 1) {
                        a->latitude.totalSec += a->latitude.minute * 60;
                        a->longitude.totalSec += a->longitude.minute * 60;
                        b->latitude.totalSec += b->latitude.minute * 60;
                        b->longitude.totalSec += b->longitude.minute * 60;
                }
                else {
                        a->latitude.totalSec += a->latitude.second;
                        a->longitude.totalSec += a->longitude.second;
                        b->latitude.totalSec += b->latitude.second;
                        b->longitude.totalSec += b->longitude.second;
                }
        }
        if (a->longitude.direction == 'W') a->longitude.totalSec = -1 * a->longitude.totalSec;
        if (a->latitude.direction == 'S') a->latitude.totalSec = -1 * a->latitude.totalSec;
        if (b->longitude.direction == 'W') b->longitude.totalSec = -1 * b->longitude.totalSec;
        if (b->latitude.direction == 'S') b->latitude.totalSec = -1 * b->latitude.totalSec;
}
// print features
void print_features(const struct coordinate* x, const struct coordinate* y){
        printf("(%03dd %02dm %02ds %c, %03dd %02dm %02ds %c) (%03dd %02dm %02ds %c, %03dd %02dm %02ds %c)",
                        x->longitude.degree, x->longitude.minute, x->longitude.second, x->longitude.direction, x->latitude.degree, x->latitude.minute, x->latitude.second, x->latitude.direction,
                        y->longitude.degree, y->longitude.minute, y->longitude.second, y->longitude.direction, y->latitude.degree, y->latitude.minute, y->latitude.second, y->latitude.direction);
}
