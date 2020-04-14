#include "mizzo.hpp"
#include "dataBuffer.hpp"

class Consumer : public Mizzo
{

public:
    void* consume(void* buffer);
    void removeCandy(std::string candy);
};

// code to be exited by threads Ethel/Lucy
void* Consumer::consume(void *b) {
//while(1)
//
/*
waits on full sempahore, can I consume?
-once available , lock the buffer, consume what it needs
-consume item (take care of stats), 'dequeue' from buffer
-unlock the buffer mutex
-increment empty semaphore
- increment frog empty semaphore if frog removed
- print out who consumed
*/

    buffer* sharedBuff = (buffer*) b;
    std::string candy;
    for (;;) {
        
        //check if there are candies to remove
        sem_wait(sharedBuff->full);
        //buffer will be modified, mutual exclusion
        pthread_mutex_lock(&(sharedBuff->lock));
        //take candy off belt - pop off queue
        candy = sharedBuff->conveyorBelt->front();
        sharedBuff->conveyorBelt->pop();
        //buffer is done being modified
        pthread_mutex_unlock(&(sharedBuff->lock));
        // check if candy was a frog bite
        if (candy.compare("crunchy frog bite") == 0)
            //if a frog bite than increment frog semaphore
            sem_post(sharedBuff->frogEmpty);
        //candy has been removed so a slot is available
        sem_post(sharedBuff->empty);

    }

}

// print out what happened
void Consumer::removeCandy(std::string candy) {
    std::cout << this->getName << " consumed a " << candy << endl;
}