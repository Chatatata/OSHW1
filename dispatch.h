//
//  dispatch.h
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//  Thread dispatch utilities
//

#ifndef dispatch_h
#define dispatch_h

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "question.h"

typedef struct dispatch_t {
    char type[60];
    unsigned int wait_len;
    unsigned int start_pos;
    unsigned int end_pos;
    unsigned int order;
} dispatch_t;

void *dispatch_invoke_f(dispatch_t *dispatcher);

#endif /* dispatch_h */
