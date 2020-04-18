#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h> /* define bool */
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include "producer.hpp"
#include "consumer.hpp"
#include "dataBuffer.hpp"

#define BUFFER_SIZE 10

using namespace std;
void initialize_data(buff *b);
void printUsage();
void produceReport();

void printUsage()
{
    std::cout << "Usage:"
              << std::endl
              << "      -E       Specifies the number of milliseconds N that the Ethel consumer requires "
              << "to put a candy in the box and should be invoked each time Ethel removes a "
              << "candy regardless of the candy type. You would simulate this time to consume a "
              << "product (put a candy in the box) by putting the consumer thread to sleep for N "
              << "milliseconds. Other consumer and producer threads (Lucy, frog bite, and "
              << "escargot sucker) are handled similarly. "
              << std::endl
              << "      -L       Similar argument for the Lucy consumer."
              << std::endl
              << "      -f       N Specifies the number of milliseconds required to produce each crunchy frog bite."
              << std::endl
              << "      -e       N Specifies the number of milliseconds required to produce each everlasting escargot sucker."
              << std::endl;

    cout << "Example: ./mizzo –E 3000000 –L 400000 -f 20000 -e 21134" << endl;
}

int main(int argc, char **argv)
{
    int option = 0;
    int ethelTimeMS = 0;
    int lucyTimeMS = 0;
    int frogTimeMS = 0;
    int escTimeMS = 0;

    while ((option = getopt(argc, argv, "E:L:f:e:")) != -1)
    {

        switch (option)
        {
        case 'E':
            /*
        N Specifies the number of milliseconds N that the Ethel consumer requires
        to put a candy in the box and should be invoked each time Ethel removes a
        candy regardless of the candy type. You would simulate this time to consume a
        product (put a candy in the box) by putting the consumer thread to sleep for N
        milliseconds. Other consumer and producer threads (Lucy, frog bite, and
        escargot sucker) are handled similarly.
        */
            ethelTimeMS = atoi(optarg);
            break;
        case 'L': /* N Similar argument for the Lucy consumer. */
            lucyTimeMS = atoi(optarg);

            break;
        case 'f': /* N Specifies the number of milliseconds required to produce each crunchy
                                frog bite. */
            frogTimeMS = atoi(optarg);
            break;
        case 'e': /*    N Specifies the number of milliseconds required to produce each
                    everlasting escargot sucker.    */
            escTimeMS = atoi(optarg);
            // No argument this time, just set a flag
            break;
        default:
            // print usage and die…
            printUsage();
        }
    }

    pthread_t frogGen;
    pthread_t escGen;
    pthread_t lucy_;
    pthread_t ethel_;

    buff *buffer = (buff *)malloc(sizeof(buff));
    initialize_data(buffer);
    //buffer->maxProd.store(0); //initialize

    mizzo *frog = (mizzo *)malloc(sizeof(mizzo));
    mizzo *escargot = (mizzo *)malloc(sizeof(mizzo));
    mizzo *lucy = (mizzo *)malloc(sizeof(mizzo));
    mizzo *ethel = (mizzo *)malloc(sizeof(mizzo));

    frog->delay = frogTimeMS;
    frog->name = "crunchy frog bite";
    frog->shared_buffer = buffer;

    escargot->delay = escTimeMS;
    escargot->name = "escargot sucker";
    escargot->shared_buffer = buffer;

    lucy->delay = lucyTimeMS;
    lucy->name = "Lucy";
    lucy->shared_buffer = buffer;

    ethel->delay = ethelTimeMS;
    ethel->name = "Ethel";
    ethel->shared_buffer = buffer;

    pthread_create(&frogGen, NULL, produce, (void *)frog);
    pthread_create(&escGen, NULL, produce, (void *)escargot);
    pthread_create(&lucy_, NULL, consume, (void *)lucy);
    pthread_create(&ethel_, NULL, consume, (void *)ethel);

    sem_wait(&buffer->finished);

    std::cout << endl;
    std::cout << "PRODUCTION REPORT" << endl;
    std::cout << endl;
    std::cout << "----------------------------------------" << endl;
    std::cout << endl;
    std::cout << "crunchy frog bite producer generated " << buffer->numFrogProduced << " candies" << endl;
    std::cout << endl;
    std::cout << "escargot sucker producer generated " << buffer->numEscProduced << " candies" << endl;
    std::cout << endl;
    std::cout << "Lucy consumed " << buffer->numLucyFrogConsumed << " crunchy frog bites + " << buffer->numLucyEscConsumed << " escargot suckers = " << (buffer->numLucyEscConsumed + buffer->numLucyFrogConsumed) << endl;
    std::cout << endl;
    std::cout << "Ethel consumed " << buffer->numEthelFrogConsumed << " crunchy frog bites + " << buffer->numEthelEscConsumed << " escargot suckers = " << (buffer->numEthelEscConsumed + buffer->numEthelFrogConsumed) << endl;
}
//initialize buffer semaphores
void initialize_data(buff *b)
{

    //initialize queue
    b->conveyorBelt = new std::queue<std::string>();

    // initialize semaphores
    // initialize empty to 10
    if (sem_init(&b->empty, 0, 10) == -1)
    {
        fprintf(stderr, "Could not initialize \"empty\" semaphore. Exiting.\n");
        exit(0);
    }

    //initialize frogEmpty to 3
    if (sem_init(&b->frogEmpty, 0, 3) == -1)
    {
        fprintf(stderr, "Could not initialize \"frogEmpty\" semaphore. Exiting.\n");
        exit(0);
    }

    //initialize full semaphore to 0
    if (sem_init(&b->full, 0, 0) == -1)
    {
        fprintf(stderr, "Could not initialize \"full\" semaphore. Exiting.\n");
        exit(0);
    }

    //initialize finished to 0
    if (sem_init(&b->finished, 0, 0) == -1)
    {
        fprintf(stderr, "Could not initialize \"finished\" semaphore. Exiting.\n");
        exit(0);
    }
}
