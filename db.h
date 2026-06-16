#ifndef _DB_H
#define _DB_H

#include "student.h"

/**
 * Database structure type.
 */
typedef struct {
    Student* data; /** The list of students **/
    size_t lsize;    /** The logical size of the list **/
    size_t psize;    /** The physical size of the list **/
} Database;

/** 
 *  Add a student to the database.
 * TODO: implement this function.
 **/
void db_add(Database *db, Student s);

/**
 * Delete a student from the database.
 * TODO: implement this function.
 **/
void db_delete(Database *db, Student *s);

/**
 * Save the content of a Database to the specified file.
 * TODO: implement this function
 **/
void db_save(Database *db, const char *path);

/**
 * Load the content of a database of students from a file.
 * TODO: implement this function.
 **/
void db_load(Database *db, const char *path);

/**
 * Initialise a Database structure.
 * Typical use:
 * ```
 * Database db;
 * db_init(&db);
 * ```
 * TODO: implement this function.
 **/
void db_init(Database *db);

#endif