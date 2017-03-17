//
//  main.c
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//
//  Student number: 150120016
//  MISRA-C compliant.
//
//  Compile with ```gcc -std=C99 main.c sema.c dispatch.c question.c -o main'''

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>

#include "sema.h"
#include "parser.h"

extern void *dispatch_invoke_f(dispatch_t *dispatcher);

question_t *questions = NULL;
unsigned int num_workers = 0;
unsigned int num_questions = 0;
unsigned int num_count = 0;
int sema = 0;
pthread_mutex_t mutex_var = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

dispatch_t **readf(int num_workers);

int main(int argc, const char * argv[]) {
    //  Seed random generator used in task creator
    srand((unsigned int)time(NULL));
    pthread_t threads[3];
    pthread_attr_t attr;
    unsigned int i = 0;
    
    //  Set POSIX thread detached attribute
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    //  Read application starting arguments given in command line
    if (argc == 2) {
        num_workers = atoi(argv[1]);
#ifdef kDEBUG_CONST
        printf("Started with %d threads.\n", num_workers);
#endif
    } else {
        //  Print usage manifest and bailout
        puts("USAGE:");
        puts("");
        puts("argument 1: number of worker threads");
        
        return 9;
    }
    
    //  Allocate storage for all questions
    questions = calloc(sizeof(question_t), num_questions);
    
    //  Read tasks from input file
    dispatch_t **dispatchers = readf(num_workers);
    
    //  Calculate number of questions going to be created
    for (i = 0; i < num_workers; ++i) {
        if (dispatchers[i]) {
            num_questions += dispatchers[i]->end_pos - dispatchers[i]->start_pos;
        }
    }
    
    //  Create a key to get semaphore
    int k_gen = ftok(argv[0], 1);
    
    //  Get semaphores with OS call
    sema = semget(k_gen, num_questions + 1, 0700|IPC_CREAT);
    
    for (i = 0; i < num_questions; ++i) {
        semctl(sema, i, SETVAL, 1);
    }
    
    semctl(sema, num_questions, SETVAL, 0);
    
    unsigned int worker = 0;
    
    //  Allocate and initialize threads and run them all
    for (worker = 0; worker < num_workers; ++worker) {
        //  Error propagator
        int propagator = 0;
        
        //  Create threads via POSIX threads API
        if ((propagator = pthread_create(&threads[worker], &attr, (void *)dispatch_invoke_f, dispatchers[worker]))) {
            fprintf(stderr, "Could not create thread: %s\n", strerror(propagator));
            
            //  Release objects
            free(questions);
            
            unsigned int a = 0;
            
            for (a = 0; a < num_workers; ++a) {
                free(dispatchers[a]);
            }
            
            free(dispatchers);
            
            unsigned int b = 0;
            
            //  Release semaphores
            for (b = 0; b < num_questions; ++b) {
                semctl(sema, b, IPC_RMID);
            }
            
            return 9;
        }
    }
    
    //  Wait for all threads to finish
    for (i = 0; i < num_workers; ++i) {
        pthread_join(threads[i], NULL);
    }

    //  Release objects
//    free(questions);
    
    for (i = 0; i < num_workers; ++i) {
        free(dispatchers[i]);
    }

    free(dispatchers);
    
    //  Release semaphores
    for (i = 0; i < num_questions; ++i) {
        semctl(sema, i, IPC_RMID);
    }
    
    return 0;
}












