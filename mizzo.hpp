
#ifndef __MIZZO_HPP__
#define __MIZZO_HPP__
       
#include <unistd.h>
#include <string>
#include <thread>
#include <chrono>
#include "dataBuffer.hpp"

using namespace std;

typedef struct factory {

    buff* shared_buffer;
    int delay;
    std::string name;

} mizzo;

#endif