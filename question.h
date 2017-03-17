//
//  question.h
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//  Question model object
//

#ifndef question_h
#define question_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <stdint.h>

typedef enum { COMP_OP_SYS, SOFT_ENG, SYS_PROG } question_type_t;

typedef struct question_t {
    char text[60];
    int approvals;
} question_t;

void question_create_f(question_t *question, const char *type, const unsigned int wait_len, const unsigned int order);
void question_chk_f(question_t *question, const int index, const unsigned int order);


#endif /* question_h */
