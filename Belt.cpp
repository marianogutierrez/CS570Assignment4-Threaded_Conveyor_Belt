#include <queue>
#include "Belt.hpp"


//Cameron Ozatalar
//Mariano Gutierrez
using namespace std;

Belt::Belt(){
    belt = new std::queue<int>();
    frogs = 0;
    escargots = 0;
    lifeTimeProduced  = 0;
    lifeTimeConsumed = 0;
    // middle val in pshared, in this case it's 0 because it will be shared between threads.
    sem_init(&(this -> barrier),0,0);  // always to zero for barriers
    sem_init(&(this -> consumeSignal),0,0);  // ready to consume signal
    sem_init(&(this ->availSlots),0,10); // ten slots available 
    sem_init(&(this ->frogSignal),0,0); // to disallow more than 3 frogbites at a time
    sem_init(&(this ->mutex),0,1); // bin semaphore  a.k.a a mutex 
}

Belt::~Belt(){
    sem_destroy(&(this -> barrier));
    sem_destroy(&(this -> consumeSignal));
    sem_destroy(&(this -> availSlots));
    sem_destroy(&(this -> frogSignal));
    sem_destroy(&(this -> mutex));
}

bool Belt::push(int itemToQueue){
     if((this -> belt -> size()) > 10) // no more than 10 items on the belt.
        return false;
    belt -> push(itemToQueue);
    lifeTimeProduced++; // end with increment 
    return true;

}

int Belt::pop(){
    int front = belt -> front();
    belt -> pop();
    lifeTimeConsumed++;
    return front;
}