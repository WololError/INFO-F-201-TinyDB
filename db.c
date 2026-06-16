#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

void db_init(Database *db) {
    db->data = malloc(INITIAL_CAPACITY * sizeof(Student));

    if (db->data == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    db->lsize = 0;
    db->psize = INITIAL_CAPACITY;
}

void db_add(Database *db, Student s) {

    if (db->lsize >= db->psize) {
        db->psize = 2 * db->psize;

        Student *temp = realloc(db->data, db->psize * sizeof(Student));

        if (temp == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        db->data = temp;
    }

    db->data[db->lsize] = s;
    db->lsize++;
}

//on remplace ledutiant par le dernier
void db_delete(Database *db, Student *s) {
    for (size_t i = 0; i < db->lsize; i++) {
        if (student_equals(&db->data[i], s)) {
            db->data[i] = db->data[db->lsize - 1];
            db->lsize--;
            return;
        }
    }
}

void db_save(Database *db, const char *path) {

    FILE* f = fopen(path, "wb");

    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return;
    }

    fwrite(db->data, sizeof(Student), db->lsize, f);

    fclose(f);
}

void db_load(Database *database, const char *path){
    FILE *file = fopen(path, "rb");
    if (!file){
        printf("Error opening file.\n");
        return;
    }
    Student* student = NULL; 
    student = malloc(sizeof(Student));
    while (fread(student, sizeof(Student), 1, file)){ 
        db_add(database, *student); 
    }
    fclose(file);
}