#pragma once

#include "prog.hpp"

// sleep for ms milliseconds
int sleep(int ms=ARG0){
    __raw(0x0A, __addr(ms));
}

// sleep for rand()%ms+1 milliseconds
// see rand() in rand.hpp
int sleep_rand(int ms=ARG0){
    __raw(0x08, __addr(ms));
}