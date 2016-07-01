#include "utils.h"
#include "common.h"

char* getTime() {
    char* s = (char*)malloc(30*sizeof(char));
    time_t t = time(NULL);
    struct tm * p = localtime(&t);
    strftime(s, 30, "%c", p);
    return s;
}