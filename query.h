#ifndef _QUERY_H
#define _QUERY_H

#include "db.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* result;
    int count;
} QueryResult;

int student_matches(Student *s, char *field, char *value);

void student_change(Student *s, char *field, char *value);

QueryResult* computeSelectQuery(Database *db, char *field, char *value);

QueryResult* computeInsertQuery(Database* db,unsigned id,const char *fname,const char *lname,const char *section,struct tm birthdate);

QueryResult* computeDeleteQuery(Database* db, char* field, char* value) ;

QueryResult* computeUpdateQuery(Database* db,char* field_filter, char* value_filter, char* field_to_update, char* update_value);

QueryResult* buildQueryResult(Student *students, int count) ;

void freeQueryResult(QueryResult *result);


#endif