#include <string>

class Producer
{
    std::string name;
    int delay;

public:
    Producer(std::string name, int delay);
    void *produce();
};

/*
semaphore 3
semaphore 10
lock for buffer

produce function:
    1- produceItem()
    2- factory waits on empty spot on conveyer belt
    3- once available, lock the buffer
    4- update 'enqueue'
    5- unlockBuffer
    6- increment semaphore of candy available
    7- 

*/