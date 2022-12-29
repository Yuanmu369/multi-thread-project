#include "interface.h"
#include "scheduler.h"
#include <unistd.h>
#include <semaphore.h>
// Interface implementation
// Implement APIs here...



// ThreadInfo *head = NULL;
ThreadInfo *g_threads; 

sem_t g_sem[MAX_NUM_SEM];

// two for loops for different inital thread set up

void init_scheduler(enum sch_type type, int thread_count)
{
    g_threads = scheduler_init(type, thread_count);
// inital set up for thread signal 

    for(int i=0; i<thread_count; i++) {
        sem_init(&g_threads[i].bin_sem, 0, 0);
        //g_threads[i].cond = PTHREAD_COND_INITIALIZER;
        //g_threads[i].mutex = PTHREAD_MUTEX_INITIALIZER;
    }
// inital set up for P and V 

    for (int i=0; i<MAX_NUM_SEM; i++) {
        sem_init(&g_sem[i], 0, 0);
    }
    
}

int cpu_me(float current_time, int tid, int remaining_time)
{
    int ret;

// it begins with the initial set up

    if (remaining_time == 0) {
        if (g_sch_type == SCH_FCFS) {
            return 0;
        } else {
            return scheduler(current_time, tid, remaining_time);
        }
    }
    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, remaining_time);
    printf("####11####### g_threads[%d].status = %d\n", tid, g_threads[tid].status);

// if the status of the thread is set to initial, we need to use scheduler add to append the thread into the ready linklist

    if(g_threads[tid].status == INIT && remaining_time>0) {
        scheduler_add(current_time, tid, remaining_time);
    }
    printf("#####22###### g_threads[%d].status = %d\n", tid, g_threads[tid].status);

// if the status of the thread is running, we can just return its time that been called

    if(g_threads[tid].status == RUNNING) {
        ret = scheduler(current_time, tid, remaining_time);
        g_current_time = ret;
        return ret;
    }
    //pthread_cond_wait(&g_threads[tid].cond, &g_threads[tid].mutex);

// the sem_wait here is for when we add the thread to the linklist, it may not be running, so we have to wait for it to be running

    sem_wait(&g_threads[tid].bin_sem);
    sleep(1);
    ret = (int)(current_time);
    ret = g_current_time > ret ? g_current_time : ret;

// set start time to ret and the system start to process, return the start time when it is finished

    g_threads[tid].start_time = ret;
    g_threads[tid].start_time ++;
    printf("%s %d: %d %d %d\n", __func__, __LINE__, ret, tid, remaining_time);
    // g_current_time = ret;
    return g_threads[tid].start_time;
}

int io_me(float current_time, int tid, int duration)
{
    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, duration);
    // sleep(duration);

// when our process time is less than the time it begins plus the duration time we will need to wait

    while(g_current_time < (int)current_time+duration) {
        ;
    }
    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, duration);
    return (int)current_time+duration;
}

int P(float current_time, int tid, int sem_id)
{

// you have to wait for V to process 

    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, sem_id);
    sem_wait(&g_sem[sem_id]);
    sleep(1);
    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, sem_id);
    
    return (int)g_current_time;
}

int V(float current_time, int tid, int sem_id)
{

// when V is processed,P will continue
    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, sem_id);
    sem_post(&g_sem[sem_id]);
    sleep(1);
    printf("%s %d: %f %d %d\n", __func__, __LINE__, current_time, tid, sem_id);
    return (int)g_current_time;
}

void end_me(int tid)
{
    return;
}
