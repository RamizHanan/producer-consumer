#ifndef __DATA_BUFFER_HPP__
#define __DATA_BUFFER_HPP__

#include <queue>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <atomic>

typedef struct thread_data
{
    std::queue<std::string> *conveyorBelt;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    sem_t empty;
    sem_t frogEmpty;
    sem_t full;
    sem_t finished;
    int totalProducedCount = 0;
    int totalConsumedCount = 0;
    int numLucyFrogConsumed = 0;
    int numLucyEscConsumed = 0;
    int numEthelFrogConsumed = 0;
    int numEthelEscConsumed = 0;
    int numFrogProduced = 0;
    int numEscProduced = 0;
    int totalBeltCount = 0;
    int beltFrogCount = 0;
    int beltEscCount = 0;
} buff;

#endif