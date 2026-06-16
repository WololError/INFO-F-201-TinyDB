#ifndef _LOG_H
#define _LOG_H

#include <time.h>
#include <stdio.h>

void writeLog(char* query, char* type, time_t timestamp, double time, int nbResult, const char* Result);


#endif