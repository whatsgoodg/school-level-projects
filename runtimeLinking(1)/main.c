#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "coordi.h"

int main(int argc, char* argv[]){

        struct coordinate x[1000];
        struct coordinate y[1000];
        // getting data from text file
        int size = getGIS(argv[1], x, y);
        // get degree, minute, second, direction, totalsec
        for (int i = 0; i < size; i++) setInfos(&x[i], &y[i]);
        // getting lib***.so
        char lib[30] = "./lib";
        strcat(lib, argv[2]);
        strncat(lib, ".so", 3);
        void* handle;
        void (*print_distance)(const struct coordinate*, const struct coordinate*);
        char* error;
        // open shared library and getting handle of it
        handle = dlopen(lib, RTLD_LAZY);
        if (!handle) {
                fprintf(stderr, "%s\n", dlerror());
                exit(1);
        }
         // get the address of function
        print_distance = dlsym(handle, "print_distance");
        if ((error = dlerror()) != NULL) {
                fprintf(stderr, "%s\n", error);
                exit(1);
        }
        // get degree, minute, second, direction, totalsecond, distance
        for (int i = 0; i < size; i++){
                print_features(&x[i], &y[i]);
                print_distance(&x[i], &y[i]);
        }
         // close library
        if (dlclose(handle) < 0) {
                fprintf(stderr, "%s\n", dlerror());
                exit(1);
        }
        return 0;
}
