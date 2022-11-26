#ifndef COORDI_H
#define COORDI_H
struct geoinfo {
        int degree;
        int minute;
        int second;
        int totalSec;
        int dms;
        char direction;
};

struct coordinate {
        struct geoinfo longitude;
        struct geoinfo latitude;
};
int getGIS(char* filename, struct coordinate* a, struct coordinate* b);
void setInfos(struct coordinate* a, struct coordinate* b);
void print_features(const struct coordinate* x, const struct coordinate* y);
#endif
