#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>
#include<semaphore.h>
#include <unistd.h>
#include "interface.h"

// Declare your own data structures and functions here...
extern int g_current_time;
extern enum sch_type g_sch_type;
enum Status{
    INIT, READY, BLOCK, RUNNING
};
typedef struct ThreadInfo{
    float current_time;
    int start_time;
    int tid;
    enum Status status;
    struct ThreadInfo *next;
    sem_t bin_sem;
    int remaining_time;
}ThreadInfo;

ThreadInfo* scheduler_init(enum sch_type type, int thread_count);
int scheduler(float current_time, int tid, int remaining_time);
int scheduler_add(float current_time, int tid, int remaining_time);







#endif
