
#ifndef __MIZZO_HPP__
#define __MIZZO_HPP__

#include <string>

class Mizzo
{
private:
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

#endif __MIZZO_HPP__