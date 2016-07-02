#include "common.h"
#include "utils.h"

char* getTime() {
    char* s = (char*)malloc(30*sizeof(char));
    time_t t = time(NULL);
    struct tm * p = localtime(&t);
    strftime(s, 30, "%c", p);
    return s;
}

/**
 * Libcurl
 */

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userdata;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}