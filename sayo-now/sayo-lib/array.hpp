#pragma once

#include "base.hpp"
#include "Rx.hpp"

// calls built-in i=malloc(i) command
#define __malloc_inplace(i)           __raw(0xF0,__addr(i))
// calls built-in i=free(i) command
#define __free_inplace(i)             __raw(0xF1,__addr(i))

// malloc memory in bytes,
// and make Rx points to the first byte
#define malloc_Rx(Rx, bytes)      {Rx=bytes;__malloc_inplace(Rx);}
// free malloced memory block begin with pointer
// returns free status (normally no need to check)
#define free_Rx(Rx)                __free_inplace(Rx)              


// malloc memory in bytes,
// returns pointer to the first byte
// `Rx = malloc(bytes)` equivalent to `malloc_Rx(Rx, bytes)`
// it's recommended to use `malloc_Rx` when used on Rx
int malloc(int bytes=__A) {
    __malloc_inplace(__A);
    return __A;
}

// free malloced memory block begin with pointer
// returns free status (normally no need to check)
// simply use `free_Rx` when pointer is a single variable 
int free(int pointer=__A) {
    __free_inplace(__A);
    return __A;
}

#define get_arr8_Rx(Rx,i,tar) { \
    Rx+=(i);        \
    tar=P8_##Rx;  \
    Rx-=(i);        \
}
#define set_arr8_Rx(Rx,i,num) {\
    Rx+=(i);        \
    P8_##Rx=(num);  \
    Rx-=(i);        \
}

#define get_arr16_Rx(Rx,i,tar) { \
    Rx+=(i)<<1;     \
    tar=P16_##Rx; \
    Rx-=(i)<<1;     \
}
#define set_arr16_Rx(Rx,i,num) {\
    Rx+=(i)<<1;     \
    P16_##Rx=(num); \
    Rx-=(i)<<1;     \
}

#define get_arr32_Rx(Rx,i,tar) { \
    Rx+=(i)<<2;     \
    tar=P32_##Rx; \
    Rx-=(i)<<2;     \
}
#define set_arr32_Rx(Rx,i,num) {\
    Rx+=(i)<<2;     \
    P32_##Rx=(num); \
    Rx-=(i)<<2;     \
}

