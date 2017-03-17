//
//  question.c
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//  Question model object
//

#include <string.h>
#include <pthread.h>

#include "question.h"
#include "sema.h"

extern int sema;
extern unsigned int num_workers;
extern unsigned int num_questions;
extern unsigned int num_count;
extern pthread_mutex_t mutex_var;
extern pthread_cond_t cond_var;

//  Returns a question with given type.
void question_create_f(question_t *question, const char *type, const unsigned int wait_len, const unsigned int order) {
    //  Copy data to question, no data race owing to discrete memory locations
    strcpy(question->text, type);
    question->approvals = 0;
    
    //  Simulate: Wait for WEC member to create a question
    sleep(wait_len);
    
    pthread_mutex_lock(&mutex_var);
    //  Propagate event to console, thread-safe
    printf("WEC member %d: a question is prepared on %s\n", order, question->text);
    
    //  Increment number of questions created, thread-safe
    num_count += 1;
    
    //  If total number of questions reached, signal conditional variable
    if (num_count == num_questions) {
        sema_signal(sema, num_questions, num_workers);
    }
    pthread_mutex_unlock(&mutex_var);
}

void question_chk_f(question_t *question, const int index, const unsigned int order) {
    //  Wait for WEC member to check a question
    sleep(1);
    
    sema_wait(sema, index, 1);
    //  This should be thread-safe.
    question->approvals += 1;
#ifdef kDEBUG_CONST
    printf("Question %d is approved: %d\n", index, question->approvals);
#endif
    if (question->approvals == num_workers - 1) {
        printf("WEC member %d: Question %d is ready\n", order, index);
    }
    sema_signal(sema, index, 1);
}

