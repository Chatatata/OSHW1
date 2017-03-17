//
//  parser.c
//  libexamwriters
//
//  Created by Buğra Ekuklu on 06.04.2016.
//  Copyright © 2016 The Digital Warehouse. All rights reserved.
//

#include "parser.h"
#include <string.h>

//  Read input files
dispatch_t **readf(int num_workers) {
    dispatch_t **dispatchers = NULL;
    int verified = 0;
    int count = 0;
    int lines = 0;
    char **lineptr = 0;
    const size_t linesiz = 60;
    char ch = 0;
    unsigned int i = 0;
    FILE *fptr = fopen("input_file", "r");
    
    if (fptr == NULL) {
        perror("File could not open: input_file");
        
        exit(11);
    }
    
    lineptr = (char **)calloc(sizeof(char *), 20);
    
    while (1) {
        if (lineptr[lines] == NULL) {
            lineptr[lines] = (char *)calloc(sizeof(char), linesiz);
        }
        
        ch = getc(fptr);
        
        if (ch == '\n') {
            count = 0;
            lines += 1;
        } else {
            lineptr[lines][count++] = ch;
        }
        
        if (ch == EOF) {
            break;
        }
    }
    
    for (i = 0; i < lines; ++i) {
        dispatch_t *dispatcher = parse_string(lineptr[i]);
        
        if (dispatcher != NULL) {
            if (dispatchers != NULL) {
                dispatchers = realloc(dispatchers, sizeof(dispatch_t *) * (verified + 1));
            } else {
                dispatchers = calloc(sizeof(dispatch_t *), 1);
            }
            
            //  Add padding to its positions
            if (verified > 0 && dispatchers[verified - 1] != NULL) {
                dispatcher->start_pos += dispatchers[verified - 1]->end_pos;
                dispatcher->end_pos += dispatchers[verified - 1]->end_pos;
            }
            
            //  Give a random wait length to dispatcher
            dispatcher->wait_len = rand() % 2 + 1;
            
            //  Assign it to array index
            dispatchers[verified] = dispatcher;
            
            //  Increment number of verified tasks
            verified += 1;
        }
    }
    
    if (verified != num_workers) {
        perror("Fatal error: Worker number inconsistency between argument list and input file.");
        
        exit(14);
    }
    
    //  Free heap-allocated objects
    for (i = 0; i < lines; ++i) {
        free(lineptr[i]);
    }
    fclose(fptr);
    free(lineptr);
    
    return dispatchers;
}

dispatch_t *parse_string(char *string) {
    dispatch_t *dispatcher = NULL;
    char number[6] = {0};
    char text[60] = {0};
    char quantity[6] = {0};
    int count = 0;
    int tmp_count = 0;
    unsigned int i = 0;
    
    //  Parse member identifier
    char ch = string[count];
    
    if (isdigit(ch)) {
        number[count] = ch;
        ch = string[++count];
        
        while (isdigit(ch)) {
            number[count] = ch;
            ch = string[++count];
        }
    } else {
        fprintf(stderr, "WARNING: Syntax error at string: %s, on character: %d\n", string, count);
        fprintf(stderr, "%c is not a valid digit.\n", ch);
        
        return NULL;
    }
    
    if (!isspace(ch)) {
        fprintf(stderr, "WARNING: Syntax error at string: %s, on character: %d\n", string, count);
        fprintf(stderr, "Expected white-space after member identifier.\n");
        
        return NULL;
    }
    
    ch = string[++count];
    
    for (i = 0; !isspace(ch); ++i) {
        if (i < 60) {
            text[tmp_count++] = ch;
            ch = string[++count];
        } else {
            fprintf(stderr, "WARNING: Syntax error at string: %s, on character: %d\n", string, count);
            fprintf(stderr, "Member name should be maximum 60 characters long.");
            
            return NULL;
        }
    }
    
    if (!isspace(ch)) {
        fprintf(stderr, "WARNING: Syntax error at string: %s, on character: %d\n", string, count);
        fprintf(stderr, "Expected white-space after member name.\n");
        
        return NULL;
    }
    
    ch = string[++count];
    tmp_count = 0;
    
    for (i = 0; ch != '\0'; ++i) {
        if (i < 3) {
            quantity[tmp_count++] = ch;
            ch = string[++count];
        } else {
            fprintf(stderr, "WARNING: Syntax error at string: %s, on character: %d\n", string, count);
            fprintf(stderr, "Quantity of questions overflows C unsigned 32-bit integer type.\n");
            
            return NULL;
        }
    }
    
    dispatcher = calloc(sizeof(dispatch_t), 1);
    
    dispatcher->order = (unsigned int)atol(number);
    strcpy(dispatcher->type, text);
    dispatcher->end_pos = (unsigned int)atol(quantity);
    
    return dispatcher;
}