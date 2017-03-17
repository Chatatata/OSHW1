//
//  dispatch.c
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//  Thread dispatch utilities
//

#include "dispatch.h"
#include "sema.h"

extern question_t *questions;
extern unsigned int num_questions;
extern unsigned int num_count;
extern int sema;

void *dispatch_invoke_f(dispatch_t *dispatcher) {
    //  Starting from start_pos, create questions with given dispatcher until end_pos is reached
    unsigned int i = 0;
    for (i = dispatcher->start_pos; i < dispatcher->end_pos; ++i) {
        question_create_f(&questions[i], dispatcher->type, dispatcher->wait_len, dispatcher->order);
    }
    
    //  Lock mutex and wait for other threads (WEC members) finish assigned questions
    sema_wait(sema, num_questions, 1);
    
    //  Approve unassigned (foreign) questions, concurrently
    for (i = 0; i < dispatcher->start_pos; ++i) {
        question_chk_f(&questions[i], i, dispatcher->order);
    }
    
    for (i = dispatcher->end_pos; i < num_questions; ++i) {
        question_chk_f(&questions[i], i, dispatcher->order);
    }
    
    //  Finish thread execution
    pthread_exit(NULL);
    
    return NULL;
}