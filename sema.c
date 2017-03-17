//
//  sema.c
//  libexamwriters
//
//  Created by Buğra Ekuklu on 05.04.2016.
//  Semaphore handling functions
//

#include "sema.h"

void sema_signal(int semid, int semnum, int value) {
    struct sembuf semaphore;
    
    semaphore.sem_num = semnum;
    semaphore.sem_op = value;
    semaphore.sem_flg = 1;
    
    semop(semid, &semaphore, 1);
}

void sema_wait(int semid, int semnum, int value) {
    struct sembuf semaphore;
    
    semaphore.sem_num = semnum;
    semaphore.sem_op = (-1 * value);
    semaphore.sem_flg = 1;
    
    semop(semid, &semaphore, 1);
}