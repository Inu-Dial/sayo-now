#pragma once

#ifndef SAYO_NOW    // to make program valid in C++ highlight
                    // in order to make g++ preprocessor ignore these, use -DSAYO_NOW option for g++

#define __addr(x) 0
#define __var(x) *((void*)x)
#define __raw(...) 0

#endif

#include "Rx.hpp"

// __A and __B must be defined to specify address of A and B
// any operator will always change A and B, and only changes A and B
// A and B should also be defined in translate_config/def.txt

int __A = 16;
int __B = 17;

// sorry for only one variable a time (;-;)
// ARG0~4: script argument,

int ARG0    = 0;
int ARG1    = 1;
int ARG2    = 2;
int ARG3    = 3;
int DPTR    = 9;
int P_DPTR  = 8;
int IO      = 10;
int NONE    = 15;

int __SYS_TIME_MS   = 18;
int __SYM_TIME_S    = 19;
int __SYS_KEY_LAY   = 22;
int __SCRIPT_ADDR   = 23;       // beginning of the script

#define exit()              __raw(0xFF)
#define restart()           __raw(0xFE)
#define enter_jog_mode()    __raw(0xF8)
#define jump_to_script(u8_constant)     __raw(0xF5,u8_constant)

#define nop()               __raw(0x01)
#define wait_update()       __raw(0xF4)

#define endl 10

#define swap(x,y) __raw(0x6A,__addr(x),__addr(y))

#define push(x)   {__A=x; __raw(0x6c,__addr(__A));}
#define pop(x)    {__raw(0x6d,__addr(x));}
