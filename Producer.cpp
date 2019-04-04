#include "Producer.hpp"
//Cameron Ozatalar
//Mariano Gutierrez
Producer::Producer(Belt* belt, int delay) {
    conveyor = belt;
    speed = delay;
    totalProduced = 0; // total produced by this particular producer
}

Producer::~Producer() {
    // nothing was malloc'd or new'd so nothing to do
}