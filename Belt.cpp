#include <queue>
#include "Belt.hpp"

//Cameron Ozatalar
//Mariano Gutierrez

Belt::Belt(){
    belt = new std::queue<int>();
    total = 0;
    crunFrogNum = 0;
    escargotNum = 0;
}

Belt::~Belt(){
    delete belt;
}

void Belt::push(int itemToQueue){
    //TODO: check type and change values
    
    belt -> push(itemToQueue);
}

int Belt::pop(){
    int front = belt -> front();
    belt -> pop();

    //TODO: check type and change values

    return front;
}