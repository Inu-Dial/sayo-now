#pragma once

#include "base.hpp"

// ms <= 65535
#define __sleep_const(ms)   __raw(0x0D,ms)
#define __sleep_val(ms)     __raw(0x0A,__addr(ms))

// sleep for ms milliseconds
int sleep(int ms=__A){
    __sleep_val(ms);
}

// sleep for rand()%ms+1 milliseconds
// see rand() in rand.hpp
int sleep_rand(int ms=__A){
    __raw(0x08, __addr(ms));
}