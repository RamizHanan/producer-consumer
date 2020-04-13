#include "mizzo.hpp"




class Consumer : public Mizzo
{

public:
    void *consume(void* buffer); //buffer data structure
};



void* Consumer::consume(void * buffer){
//dequeue from buffer
//while(1)
//
/*
waits on full sempahore, can I consume?
-once available , lock the buffer, consume what it needs
-consume item (take care of stats), 'dequeue' from buffer
-unlock the buffer mutex
-increment empty semaphore
- increment frog empty semaphore if frog removed
//print out who consumed

*/

}