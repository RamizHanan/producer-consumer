#ifndef __CONSUMER_HPP__
#define __CONSUMER_HPP__

#include "mizzo.hpp"
#define MAX 100
/*
-waits on full sempahore, can I consume?
-once available , lock the buffer, consume what it needs
-consume item (take care of stats), 'dequeue' from buffer
-unlock the buffer mutex
-increment empty semaphore
-increment frog empty semaphore if frog removed
-print out who consumed
*/
void* consume(void* args);
void consumeCandy(std::string candy, std::string name, buff* sharedBuff);
void* consume(void* args) {
    // cast void* to buffer*
    mizzo* worker = (mizzo*) args;
    buff* sharedBuff = worker->shared_buffer;
    std::string name = worker->name;
    int delay = worker->delay;
    // declare candy, will be initialized based on what gets popped off the queue
    std::string candy;
    while(sharedBuff->maxProd < MAX){    
        //check if there are candies to remove
        sem_wait(&sharedBuff->full);

        // critical section

        //buffer will be modified, mutual exclusion
        pthread_mutex_lock(&(sharedBuff->lock));
        //get first in queue
        candy = sharedBuff->conveyorBelt->front();
        //pop queue (dequeue)
        sharedBuff->conveyorBelt->pop();
        sharedBuff->totalBeltCount--;
        // decrement either candy based on the thread (object) executing
        if (candy.compare("crunchy frog bite") == 0) {
            sharedBuff->beltFrogCount--;
            sharedBuff->numFrogConsumed++;
        }
        else {
            sharedBuff->beltEscCount--;
            sharedBuff->numEscConsumed++;
        }
        //print out what happened with current values before they could change
        consumeCandy(candy, name, sharedBuff);
        sharedBuff->totalConsumedCount++;
        //buffer is done being modified
        pthread_mutex_unlock(&(sharedBuff->lock));

        // end critical section

        //check if candy was a frog bite
        if (candy.compare("crunchy frog bite") == 0)
            //if a frog bite than increment frog semaphore
            sem_post(&sharedBuff->frogEmpty);
        //candy has been removed so a slot is empty
        sem_post(&sharedBuff->empty);

        if(delay > 0)
            usleep(delay * 1000); //milliseconds

    }

    return NULL;
}

// print out what happened
void consumeCandy(std::string candy, std::string name, buff* sharedBuff) {
    std::cout << "Belt: " << sharedBuff->beltFrogCount << " frogs + " << sharedBuff->beltEscCount << " escargots = ";
    std::cout << sharedBuff->totalBeltCount << ". produced: " << sharedBuff->totalProducedCount;
    std::cout << "\t" << name << " consumed a " << candy << "." << std::endl;
}

#endif 