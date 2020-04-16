#ifndef __CONSUMER_HPP__
#define __CONSUMER_HPP__

#include "mizzo.hpp"


class Consumer : public Mizzo
{
private:
    int numEscConsumed = 0;
    int numFrogConsumed = 0;
public:
    void* consume(void* b);
    void consumeCandy(std::string candy, void* b);
    int getNumEscConsumed(){return this->numEscConsumed;};
    int getNumFrogConsumed(){return this->numFrogConsumed;};
};

/*
-waits on full sempahore, can I consume?
-once available , lock the buffer, consume what it needs
-consume item (take care of stats), 'dequeue' from buffer
-unlock the buffer mutex
-increment empty semaphore
-increment frog empty semaphore if frog removed
-print out who consumed
*/

void* Consumer::consume(void* b) {

    // cast void* to buffer*
    buff* sharedBuff = (buff*) b;
    // declare candy, will be initialized based on what gets popped off the queue
    std::string candy;
    for (;;) {
        
        //check if there are candies to remove
        sem_wait(sharedBuff->full);

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
            this->numFrogConsumed++;
        }
        else {
            sharedBuff->beltEscCount--;
            this->numEscConsumed++;
        }
        //print out what happened with current values before they could change
        this->consumeCandy(candy, b);
        //buffer is done being modified
        pthread_mutex_unlock(&(sharedBuff->lock));

        // end critical section

        //check if candy was a frog bite
        if (candy.compare("crunchy frog bite") == 0)
            //if a frog bite than increment frog semaphore
            sem_post(sharedBuff->frogEmpty);
        //candy has been removed so a slot is empty
        sem_post(sharedBuff->empty);

        if(this->delay > 0)
            usleep(this->delay * 1000); //milliseconds

    }

}

// print out what happened
void Consumer::consumeCandy(std::string candy, void* b) {
    buff* sharedBuff = (buff*) b;
    std::cout << "Belt: " << sharedBuff->beltFrogCount << " frogs + " << sharedBuff->beltEscCount << " escargots =";
    std::cout << sharedBuff->totalBeltCount << ". produced: " << sharedBuff->totalCount;
    std::cout << "\t" << this->getName() << " consumed a " << candy << "." << endl;
}

#endif __CONSUMER_HPP__