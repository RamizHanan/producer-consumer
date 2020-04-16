#ifndef __DATA_BUFFER_HPP__
#define __DATA_BUFFER_HPP__

#include <queue>
#include <string>
#include <pthread.h>
#include <semaphore.h>

typedef struct thread_data
{
    std::queue<std::string> *conveyorBelt;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    sem_t *empty;
    sem_t *frogEmpty;
    sem_t *full;

    // keep track of candy produced
    int totalCount = 0;
    int totalBeltCount = 0;
    int beltFrogCount = 0;
    int beltEscCount = 0;
} buff;

//initialize buffer semaphores
void initialize_data(buff *b)
{

    //initialize queue
    b->conveyorBelt = new std::queue<std::string>();

    // initialize semaphores
    // initialize empty to 10
    if (sem_init(b->empty, 0, 10) == -1)
    {
        fprintf(stderr, "Could not initialize \"empty\" semaphore. Exiting.\n");
        exit(0);
    }

    //initialize frogEmpty to 3
    if (sem_init(b->frogEmpty, 0, 3) == -1)
    {
        fprintf(stderr, "Could not initialize \"frogEmpty\" semaphore. Exiting.\n");
        exit(0);
    }

    //initialize full semaphore to 0
    if (sem_init(b->empty, 0, 0) == -1)
    {
        fprintf(stderr, "Could not initialize \"full\" semaphore. Exiting.\n");
        exit(0);
    }
}

#endif __DATA_BUFFER_HPP__