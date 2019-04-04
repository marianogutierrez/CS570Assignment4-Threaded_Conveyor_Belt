#include "Consumer.hpp"
//Cameron Ozatalar
//Mariano Gutierrez
Consumer::Consumer(Belt* belt, string name) {
    this -> name = name;
    this -> conveyor = belt;
    frogBiteConsumed = 0;
    escargotConsumed = 0;
    totalConsumed = 0;
}

Consumer::~Consumer() {
    // nothing was malloc'd or new'd so nothing to do
}