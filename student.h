#ifndef _STUDENT_H
#define _STUDENT_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * Student structure type.
 **/
typedef struct {
    unsigned id;         /** Unique ID **/
    char fname[64];      /** Firstname **/
    char lname[64];      /** Lastname **/
    char section[64];    /** Section **/
    struct tm birthdate; /** Birth date **/
} Student;

/**
 * Convert a student to a human-readlable string.
 **/
void student_to_str(char* buffer, Student* s);

/**
 * Return whether two students are equal or not.
 * Two students are equal when all their fields are equal.
 **/
int student_equals(Student* s1, Student* s2);

Student create_student(
    unsigned id,
    const char *fname,
    const char *lname,
    const char *section,
    struct tm birthdate
);

#endif