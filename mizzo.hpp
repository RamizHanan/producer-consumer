
#ifndef __MIZZO_HPP__
#define __MIZZO_HPP__
       
#include <unistd.h>
#include <string>
#include <thread>
#include <chrono>
#include "dataBuffer.hpp"


class Mizzo
{
protected:
    std::string name;
    int delay;
    const int beltSize = 10;

public:
    Mizzo(std::string name, int delay);
    std::string getName(){return this->name;};
};



Mizzo::Mizzo(std::string name, int delay){
    this->name = name;
    this->delay = delay;
}

#endif