#pragma once

#include "base.hpp"

int __RANDOM = 24;

// set seed for rand()
int srand(int seed){
    __RANDOM=seed;
}
// get a random number
#define rand() __RANDOM