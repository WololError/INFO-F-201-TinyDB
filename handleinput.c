#include "handleinput.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void *handleUserInput(void *arg) {
    DatabaseAndQuery *data = (DatabaseAndQuery *) arg;

    char query_work[256];
    char query_copy[256];

    strncpy(query_work, data->query, sizeof(query_work) - 1);
    query_work[sizeof(query_work) - 1] = '\0';

    strncpy(query_copy, data->query, sizeof(query_copy) - 1);
    query_copy[sizeof(query_copy) - 1] = '\0';

    command_t cmd = get_command_type(query_work);

    char *args = query_work;
    strtok_r(args, " ", &args);


    switch (cmd) {
        case CMD_SELECT:
            handleSelect(data->db, args, query_copy, data->mutex);
            break;

        case CMD_INSERT:
            handleInsert(data->db, args, data->db_path, query_copy, data->mutex);
            break;

        case CMD_DELETE:
            handleDelete(data->db, args, data->db_path, query_copy, data->mutex);
            break;

        case CMD_UPDATE:
            handleUpdate(data->db, args, data->db_path, query_copy, data->mutex);
            break;

        case CMD_UNKNOWN:
        default:
            fprintf(stderr, "Unknown command\n");
            break;
    }

    free(data);
    return NULL;
}

void handleSelect(Database *db, char *args, char *query_copy, pthread_mutex_t *mutex) {
    char field[64];
    char value[64];

    if (parse_selectors(args, field, value)) {
        clock_t start = clock();

        pthread_mutex_lock(mutex);
        QueryResult *result = computeSelectQuery(db, field, value);
        pthread_mutex_unlock(mutex);

        clock_t end = clock();
        double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        writeLog(query_copy, "select", time(NULL), temps_ms, result->count, result->result);
        freeQueryResult(result);
    } else {
        fprintf(stderr, "Invalid select query\n");
    }
}

void handleInsert(Database *db, char *args, const char *db_path, char *query_copy, pthread_mutex_t *mutex) {
    char fname[64];
    char lname[64];
    char section[64];
    unsigned id;
    struct tm birthdate = {0};

    if (parse_insert(args, fname, lname, &id, section, &birthdate)) {
        clock_t start = clock();

        pthread_mutex_lock(mutex);

        QueryResult *result = computeInsertQuery(db, id, fname, lname, section, birthdate);

        if (db_path != NULL) {
            db_save(db, db_path);
        }

        pthread_mutex_unlock(mutex);

        clock_t end = clock();
        double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        writeLog(query_copy, "insert", time(NULL), temps_ms, result->count, result->result);
        freeQueryResult(result);
    } else {
        fprintf(stderr, "Invalid insert query\n");
    }
}

void handleDelete(Database *db, char *args, const char *db_path, char *query_copy, pthread_mutex_t *mutex) {
    char field[64];
    char value[64];

    if (parse_selectors(args, field, value)) {
        clock_t start = clock();

        pthread_mutex_lock(mutex);

        QueryResult *result = computeDeleteQuery(db, field, value);

        if (db_path != NULL) {
            db_save(db, db_path);
        }

        pthread_mutex_unlock(mutex);

        clock_t end = clock();
        double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        writeLog(query_copy, "delete", time(NULL), temps_ms, result->count, result->result);
        freeQueryResult(result);
    } else {
        fprintf(stderr, "Invalid delete query\n");
    }
}

void handleUpdate(Database *db, char *args, const char *db_path, char *query_copy, pthread_mutex_t *mutex) {
    char field_filter[64];
    char value_filter[64];
    char field_to_update[64];
    char update_value[64];

    if (parse_update(args, field_filter, value_filter, field_to_update, update_value)) {
        clock_t start = clock();

        pthread_mutex_lock(mutex);

        QueryResult *result = computeUpdateQuery(
            db,
            field_filter,
            value_filter,
            field_to_update,
            update_value
        );

        if (db_path != NULL) {
            db_save(db, db_path);
        }

        pthread_mutex_unlock(mutex);

        clock_t end = clock();
        double temps_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        writeLog(query_copy, "update", time(NULL), temps_ms, result->count, result->result);
        freeQueryResult(result);
    } else {
        fprintf(stderr, "Invalid update query\n");
    }
}