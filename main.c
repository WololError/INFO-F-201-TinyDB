#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>

#include "db.h"
#include "handleinput.h"

#define MAX_THREADS 1024
#define QUERY_MAX_LENGHT 256

volatile sig_atomic_t running = 1;

void handleEnd(int signal) {
    (void) signal;
    running = 0;
}

int main(int argc, char const* argv[]) {

    signal(SIGINT , handleEnd);
    struct sigaction sa;

    sa.sa_handler = handleEnd;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    char file[256];

    if (argc == 2) {
        strncpy(file, argv[1], sizeof(file) - 1);
        file[sizeof(file) - 1] = '\0';
    } else if (argc == 1) {
        mkdir("temp", 0755);
        snprintf(file, sizeof(file), "temp/db.bin");
    } else {
        fprintf(stderr, "Usage: %s [database_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    mkdir("logs", 0755);

    Database db;
    db_init(&db);

    fprintf(stderr, "Welcome to the Tiny Database!\n");
    fprintf(stderr, "Loading the database...\n");

    if (argc == 2) {
        db_load(&db, file);
    }

    fprintf(stderr, "Done!\n");

    pthread_mutex_t db_mutex;
    pthread_mutex_init(&db_mutex, NULL);

    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    char query[QUERY_MAX_LENGHT];

    while (running) {

        fprintf(stderr, "Please enter your requests.\n");
        fprintf(stderr, "> ");

        if ((fgets(query, sizeof(query), stdin) == NULL)) {
            break;
        }

        query[strcspn(query, "\n")] = '\0';

        if (query[0] == '\0') {
            continue;
        }

        if (thread_count >= MAX_THREADS) {
            fprintf(stderr, "Too many requests\n");
            break;
        }

        DatabaseAndQuery* data = malloc(sizeof(DatabaseAndQuery));

        if (data == NULL) {
            perror("malloc");
            break;
        }

        data->db = &db;

        strncpy(data->query, query, sizeof(data->query) - 1);
        data->query[sizeof(data->query) - 1] = '\0';

        data->db_path = file;
        data->mutex = &db_mutex;

        if (pthread_create(&threads[thread_count], NULL, handleUserInput, data) != 0) {
            perror("pthread_create");
            free(data);
            break;
        }

        thread_count++;
        fprintf(stderr, "Running query '%s'\n", data->query);
    }

    fprintf(stderr, "\n");
    fprintf(stderr, "Waiting for requests to terminate...\n");

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    fprintf(stderr, "Comitting database changes to the disk...\n");

    db_save(&db, file);

    fprintf(stderr, "Done.\n");


    pthread_mutex_destroy(&db_mutex);

    return EXIT_SUCCESS;
}