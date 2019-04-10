#pragma once
#include <string>
#include "Belt.hpp"
//Cameron Ozatalar
//Mariano Gutierrez
using namespace std;

class Consumer {
    public:
        string name; 
        Belt* conveyor;
        int delay; // the time delay 
        int escargotConsumed; // escargots eaten by this cosumer
        int frogBiteConsumed; // frogbites eaten by this consumer
        int totalConsumed; // escargot  + frogbites 
        Consumer(Belt* belt, int delay, string name);
        ~Consumer();
};