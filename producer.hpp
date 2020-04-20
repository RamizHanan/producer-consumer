#ifndef __PRODUCER_HPP__
#define __PRODUCER_HPP__

#include "mizzo.hpp"
#define MAX_PRODUCED 100

void *produce(void *args);
void produceCandy(std::string candy, buff *sharedBuff);

/*
producer thread:
    1- factory waits on empty spot on conveyer belt
    2- once available, lock the buffer
    3- update 'enqueue'
    4- produceItem() - will go in critical section
    5- unlockBuffer
    6- increment semaphore of candy available
*/

void *produce(void *args)
{
    mizzo *candyType = (mizzo *)args;
    buff *sharedBuff = candyType->shared_buffer;
    std::string candy = candyType->name;
    int delayMS = candyType->delay;
    bool done = false;
    while (!done)
    {
        //check if there are slots to place candies on
        sem_wait(&sharedBuff->empty);

        // check if candy was a frog bite
        if (candy.compare("crunchy frog bite") == 0)
            //if a frog bite than decrement frog semaphore
            sem_wait(&sharedBuff->frogEmpty);

        //buffer will be modified, mutual exclusion
        //critical section
        pthread_mutex_lock(&(sharedBuff->lock));

        // wait if there is a delay, simulates amount of time to produce
        if (delayMS > 0)
            usleep(delayMS * 1000);
        if (sharedBuff->totalProducedCount < MAX_PRODUCED)
        {
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
        }
        else 
            done = true;
        //buffer is done being modified
        
        //end critical section
        pthread_mutex_unlock(&(sharedBuff->lock));

        //candy has been added so a slot is full
        sem_post(&sharedBuff->full);
        //sharedBuff->maxProd++;
    }
    pthread_exit(NULL);
}

void produceCandy(std::string candy, buff *sharedBuff)
{
    std::cout << "Belt: " << sharedBuff->beltFrogCount << " frogs + " << sharedBuff->beltEscCount << " escargots = ";
    std::cout << sharedBuff->totalBeltCount << ". produced: " << sharedBuff->totalProducedCount;
    std::cout << "\tAdded a " << candy << "." << std::endl;
}

#endif