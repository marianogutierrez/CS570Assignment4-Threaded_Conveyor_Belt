#include <queue>
#include "Belt.hpp"

//Cameron Ozatalar

Belt::Belt(){
    belt = new std::queue<int>();
    total = 0;
    crunFrogNum = 0;
    escargotNum = 0;
}

Belt::~Belt(){
    delete belt;
}