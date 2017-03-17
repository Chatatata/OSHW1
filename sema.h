//
//  sema.h
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//  Semaphore handling functions
//

#ifndef sema_h
#define sema_h

#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdint.h>

void sema_signal(int semid, int semnum, int value);
void sema_wait(int semid, int semnum, int value);

#endif /* sema_h */
