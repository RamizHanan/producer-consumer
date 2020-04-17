#ifndef __PRODUCER_HPP__
#define __PRODUCER_HPP__

#include "mizzo.hpp"
#define MAX 100
/*
produce function:
    1- factory waits on empty spot on conveyer belt
    2- once available, lock the buffer
    3- update 'enqueue'
    4- produceItem() - will go in critical section
    5- unlockBuffer
    6- increment semaphore of candy available

*/
void* produce(void* args);
void produceCandy(std::string candy, buff* sharedBuff);

void* produce(void* args)
{
    // cast void* to buffer*
    mizzo* candyType = (mizzo*) args;
    buff* sharedBuff = candyType->shared_buffer;
    // candy produced is based on the object (thread) producing it
    std::string candy = candyType->name;
    int delay = candyType->delay;
    while(sharedBuff->maxProd < MAX - 1)
    {

        //check if there are slots to place candies on
        sem_wait(&sharedBuff->empty);

        // check if candy was a frog bite
        if (candy.compare("crunchy frog bite") == 0)
            //if a frog bite than decrement frog semaphore
            sem_wait(&sharedBuff->frogEmpty);

        if (delay > 0)
            usleep(delay * 1000);;

        //critical section

        //buffer will be modified, mutual exclusion
        pthread_mutex_lock(&(sharedBuff->lock));
        //place candy on belt - push onto queue (enqueue)
        sharedBuff->conveyorBelt->push(candy);
        sharedBuff->totalBeltCount++;
        sharedBuff->totalProducedCount++;

        // increment either candy based on the thread (object) executing
        if (candy.compare("crunchy frog bite") == 0)
        {
            sharedBuff->beltFrogCount++;
            sharedBuff->numFrogProduced++;
        }
        else
        {
            sharedBuff->beltEscCount++;
            sharedBuff->numEscProduced++;
        }
        //print out what happens before it can change
        produceCandy(candy, sharedBuff);
        //buffer is done being modified
        pthread_mutex_unlock(&(sharedBuff->lock));

        //end critical section

        //candy has been added so a slot is full
        sem_post(&sharedBuff->full);
        sharedBuff->maxProd++;
    }
}

void produceCandy(std::string candy, buff* sharedBuff)
{
    std::cout << "Belt: " << sharedBuff->beltFrogCount << " frogs + " << sharedBuff->beltEscCount << " escargots = ";
    std::cout << sharedBuff->totalBeltCount << ". produced: " << sharedBuff->totalProducedCount;
    std::cout << "\tMizzo produced a " << candy << "." << std::endl;
}

#endif