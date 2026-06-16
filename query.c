#include "query.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int student_matches(Student *s, char *field, char *value) {
    if (strcmp(field, "fname") == 0) {
        return strcmp(s->fname, value) == 0;
    }

    if (strcmp(field, "lname") == 0) {
        return strcmp(s->lname, value) == 0;
    }

    if (strcmp(field, "section") == 0) {
        return strcmp(s->section, value) == 0;
    }

    if (strcmp(field, "id") == 0) {
        return s->id == (unsigned) atoi(value);
    }

    if (strcmp(field, "birthdate") == 0) {
        char date[32];

        snprintf(date, sizeof(date), "%d/%d/%d", s->birthdate.tm_mday, s->birthdate.tm_mon + 1, s->birthdate.tm_year + 1900);

        return strcmp(date, value) == 0;
    }

    return 0;
}

void student_change(Student *s, char *field, char *value) {
    if (strcmp(field, "fname") == 0) {
        strcpy(s->fname, value);
        return;
    }

    if (strcmp(field, "lname") == 0) {
        strcpy(s->lname, value);
        return;
    }

    if (strcmp(field, "section") == 0) {
        strcpy(s->section, value);
        return;
    }

    if (strcmp(field, "id") == 0) {
        s->id = (unsigned int) atoi(value);
        return;
    }
}

QueryResult* computeSelectQuery(Database *db, char *field, char *value) {
    Student *result = malloc(db->lsize * sizeof(Student));

    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int j = 0;

    for (size_t i = 0; i < db->lsize; i++) {
        if (student_matches(&db->data[i], field, value)) {
            result[j] = db->data[i];
            j++;
        }
    }

    QueryResult *qr = buildQueryResult(result, j);

    free(result);

    return qr;
}

QueryResult* computeDeleteQuery(Database* db, char* field, char* value) {
    Student *result = malloc(db->lsize * sizeof(Student));

    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t i = 0;
    int j = 0;

    while (i < db->lsize) {
        if (student_matches(&db->data[i], field, value)) {
            result[j] = db->data[i];
            j++;

            db_delete(db, &db->data[i]);
        } else {
            i++;
        }
    }

    QueryResult *qr = buildQueryResult(result, j);

    free(result);

    return qr;
}

QueryResult* computeUpdateQuery(
    Database* db,
    char* field_filter,
    char* value_filter,
    char* field_to_update,
    char* update_value
) {
    Student *result = malloc(db->lsize * sizeof(Student));

    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int j = 0;

    for (size_t i = 0; i < db->lsize; i++) {
        if (student_matches(&db->data[i], field_filter, value_filter)) {
            student_change(&db->data[i], field_to_update, update_value);

            result[j] = db->data[i];
            j++;
        }
    }

    QueryResult *qr = buildQueryResult(result, j);

    free(result);

    return qr;
}

QueryResult* computeInsertQuery(
    Database* db,
    unsigned id,
    const char *fname,
    const char *lname,
    const char *section,
    struct tm birthdate
) {
    Student new_student = create_student(id, fname, lname, section, birthdate);

    db_add(db, new_student);

    Student result[1];
    result[0] = new_student;

    return buildQueryResult(result, 1);
}

QueryResult* buildQueryResult(Student *students, int count) {
    QueryResult* queryResult = malloc(sizeof(QueryResult));

    if (queryResult == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    queryResult->count = count;

    size_t size = ((size_t) count * 256) + 1;

    queryResult->result = malloc(size);

    if (queryResult->result == NULL) {
        perror("malloc");
        free(queryResult);
        exit(EXIT_FAILURE);
    }

    queryResult->result[0] = '\0';

    for (int i = 0; i < count; i++) {
        char buffer[256];

        student_to_str(buffer, &students[i]);

        strcat(queryResult->result, buffer);
        strcat(queryResult->result, "\n");
    }

    return queryResult;
}

void freeQueryResult(QueryResult *result) {
    if (result == NULL) {
        return;
    }

    free(result->result);
    free(result);
}