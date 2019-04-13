#pragma once // headfiler gaurd
#include <queue>
#include <semaphore.h>


//Cameron Ozatalar
//Mariano Gutierrez

class Belt{
    public:
        std::queue<int> *belt; // will need since candies are removed in FIFO
        int frogs; // number ON the belt
        int escargots; // number ON the belt
        // note also each candy is represented by an integer, specifed in the main file
        // Or, I guess I could put it here
        int lifeTimeProduced; // should add up to 100 each time.
        int lifeTimeConsumed; // again should add up to 100 this will be used to signal the barrier.
        sem_t barrier; // the barrier for printing the production report
        sem_t consumeSignal; // to signal that the belt has candy
        sem_t availSlots; // how many slots the belt has out of 10
        sem_t frogSignal; // signal to disallow more than 3 candies 
        sem_t mutex; // standard mutual exlcusion 
        Belt();
        ~Belt();
        bool push(int itemToQueue);
        int pop();
};