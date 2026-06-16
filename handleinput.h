#ifndef _HANDLE_INPUT_H
#define _HANDLE_INPUT_H

#include <pthread.h>

#include "db.h"
#include "command.h"
#include "parsing.h"
#include "query.h"
#include "log.h"

typedef struct {
    Database *db;
    char query[256];
    const char *db_path;
    pthread_mutex_t *mutex;
} DatabaseAndQuery;

void *handleUserInput(void *arg);

void handleSelect(Database *db, char *args, char *query_copy, pthread_mutex_t *mutex);
void handleInsert(Database *db, char *args, const char *db_path, char *query_copy, pthread_mutex_t *mutex);
void handleDelete(Database *db, char *args, const char *db_path, char *query_copy, pthread_mutex_t *mutex);
void handleUpdate(Database *db, char *args, const char *db_path, char *query_copy, pthread_mutex_t *mutex);

#endif