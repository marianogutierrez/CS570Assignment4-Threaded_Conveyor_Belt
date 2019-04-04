#pragma once
#include "Belt.hpp"

class Producer{
    public:
    Belt* conveyor;
    int speed; // the delay in ms
    int totalProduced;
    Producer(Belt* belt, int delay);
    ~Producer();
};