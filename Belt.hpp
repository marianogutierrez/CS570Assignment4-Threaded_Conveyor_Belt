#pragma once // headfiler gaurd
#include <queue>
#include <semaphore.h>
#include "Consumer.hpp"


//Cameron Ozatalar
//Mariano Gutierrez

class Belt{
    public:
        std::queue<int> *belt; // will need since candies are removed in FIFO
        int frogs; // number ON the belt
        int escargots; // number ON the belt
        // note also each candy is represented by an integer, specifed in the main file
        int lifeTimeProduced; // should add up to 100 each time.
        sem_t barrier;
        sem_t consumeKey;
        sem_t produceKey;
        Belt();
        ~Belt();
        bool push(int itemToQueue);
        int pop();
};