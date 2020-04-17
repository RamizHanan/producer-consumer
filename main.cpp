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

typedef void* (*PRODUCE)(void*);
typedef void* (*CONSUME)(void*);

int main(int argc, char **argv)
{
    int option = 0;
    int idx = -1;
    int ethelTimeMS;
    int lucyTimeMS;
    int frogTimeMS;
    int escTimeMS;

    if (argc < 2)
    {
        ethelTimeMS = 0;
        lucyTimeMS = 0;
        frogTimeMS = 0;
        escTimeMS = 0;
        //set default values;
    }

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

    Producer *frogBite = new Producer("crunchy frog bite", frogTimeMS);
    Producer *escargot = new Producer("escargot suckers", escTimeMS);

    Consumer *ethel = new Consumer("Ethel", ethelTimeMS);
    Consumer *lucy = new Consumer("Lucy", lucyTimeMS);

    pthread_create(&frogGen, NULL, (PRODUCE) &frogBite->produce, (void *)buffer);
    pthread_create(&escGen, NULL, (PRODUCE) &escargot->produce, (void *)buffer);
    pthread_create(&lucy_, NULL, (CONSUME) &ethel->consume, (void *)buffer);
    pthread_create(&ethel_, NULL, (CONSUME) &lucy->consume, (void *)buffer);

    //crunchy
    //escargot
    //lucy
    //ethel
}

void production_report();