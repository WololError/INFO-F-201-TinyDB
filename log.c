#include "log.h"
#include <time.h>

void writeLog(char* query, char* type, time_t timestamp, double time, int nbResult, const char* Result) {
    char filename[256];

    snprintf(filename, sizeof(filename), "logs/%lld-%s.txt", (long long)timestamp, type);

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("fopen");
        return;
    }

    fprintf(file, "Query \"%s\" completed in %.6fms with %d results.\n", query, time, nbResult);
    fprintf(file, "%s", Result);


    fclose(file);
}