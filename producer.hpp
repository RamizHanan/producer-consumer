#ifndef __PRODUCER_HPP__
#define __PRODUCER_HPP__

#include "mizzo.hpp"

class Producer : public Mizzo
{
private:
    int numEscProduced = 0;
    int numFrogProduced = 0;

public:
    void *produce(void *b);
    void produceCandy(void *b);
    int getNumEscProduced() { return this->numEscProduced; };
    int getNumFrogProduced() { return this->numFrogProduced; };
};

/*
produce function:
    1- factory waits on empty spot on conveyer belt
    2- once available, lock the buffer
    3- update 'enqueue'
    4- produceItem() - will go in critical section
    5- unlockBuffer
    6- increment semaphore of candy available

*/

void *Producer::produce(void *b)
{

    // cast void* to buffer*
    buff *sharedBuff = (buff *)b;
    // candy produced is based on the object (thread) producing it
    std::string candy = this->getName();
    for (;;)
    {
        
        //check if there are slots to place candies on
        sem_wait(sharedBuff->empty);

        // check if candy was a frog bite
        if (candy.compare("crunchy frog bite") == 0)
            //if a frog bite than decrement frog semaphore
            sem_wait(sharedBuff->frogEmpty);

        if (this->delay > 0)
            usleep(this->delay * 1000);;

        //critical section

        //buffer will be modified, mutual exclusion
        pthread_mutex_lock(&(sharedBuff->lock));
        //place candy on belt - push onto queue (enqueue)
        sharedBuff->conveyorBelt->push(candy);
        sharedBuff->totalBeltCount++;
        sharedBuff->totalCount++;
        // increment either candy based on the thread (object) executing
        if (this->getName().compare("crunchy frog bite") == 0)
        {
            sharedBuff->beltFrogCount++;
            this->numFrogProduced++;
        }
        else
        {
            sharedBuff->beltEscCount++;
            this->numEscProduced++;
        }
        //print out what happens before it can change
        this->produceCandy(b);
        //buffer is done being modified
        pthread_mutex_unlock(&(sharedBuff->lock));

        //end critical section

        //candy has been added so a slot is full
        sem_post(sharedBuff->full);
    }
}

void Producer::produceCandy(void *b)
{
    buff *sharedBuff = (buff *)b;
    std::cout << "Belt: " << sharedBuff->beltFrogCount << " frogs + " << sharedBuff->beltEscCount << " escargots =";
    std::cout << sharedBuff->totalBeltCount << ". produced: " << sharedBuff->totalCount;
    std::cout << "\tAdded a " << this->getName() << "." << endl;
}

#endif __PRODUCER_HPP__