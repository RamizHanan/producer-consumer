#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h> /* define bool */
#include <string>
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <pthread.h>
//usleep(microseconds);

#define OOPS 5

using namespace std;

void printUsage()
{
    cout << "Usage:"
         << endl
         << "      -E       Specifies the number of milliseconds N that the Ethel consumer requires "
         <<                 "to put a candy in the box and should be invoked each time Ethel removes a "
         <<                 "candy regardless of the candy type. You would simulate this time to consume a "
         <<                 "product (put a candy in the box) by putting the consumer thread to sleep for N "
         <<                 "milliseconds. Other consumer and producer threads (Lucy, frog bite, and "
         <<                 "escargot sucker) are handled similarly. "
         << endl
         << "      -L       Similar argument for the Lucy consumer."
         << endl
         << "      -f       N Specifies the number of milliseconds required to produce each crunchy frog bite."
         << endl
         << "      -e       N Specifies the number of milliseconds required to produce each everlasting escargot sucker."
         << endl;

    cout << "Example: ./mizzo –E 3000000 –L 400000 -f 20000 -e 21134" << endl;
}

int main(int argc, char **argv)
{
    int option = 0;
    int idx = -1;
    int ethelTimeMS = 0;
    int lucyTimeMS = 0;
    int frogTimeMS = 0;
    int escTimeMS = 0;

    std::string fileName = "";
    bool showTrans = false;

    if (argc < 2)
    {
        cerr << "default" << endl;
        //default setting

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
        case 'L':               /* N Similar argument for the Lucy consumer. */
            lucyTimeMS = atoi(optarg);

            break;
        case 'f':               /* N Specifies the number of milliseconds required to produce each crunchy
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
            exit(0);
        }
    }
    /* first mandatory argument, optind is defined by getopt */
    idx = optind;


}