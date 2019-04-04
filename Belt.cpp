#include <queue>
#include "Belt.hpp"


//Cameron Ozatalar
//Mariano Gutierrez
using namespace std;

Belt::Belt(){
    belt = new std::queue<int>();
    frogs = 0;
    escargots = 0;
    // middle val in pshared, in this case it's 0 because it will be shared between threads.
    sem_init(&(this -> barrier),0,0);  // always to zero for barriers
    sem_init(&(this -> produceKey),0,1); // bin. semaphore
    lifeTimeProduced  = 0;
}

Belt::~Belt(){
    sem_destroy(&(this -> barrier));
    sem_destroy(&(this -> produceKey));
}

bool Belt::push(int itemToQueue){
    //TODO: check type and change values
    if(belt -> size > 10) // no more than 10 items on the belt.
        return false;
    belt -> push(itemToQueue);
    lifeTimeProduced++; // end with increment 
    return true;
}

int Belt::pop(){
    int front = belt -> front();
    belt -> pop();
    //TODO: check type and change values
    return front;
}