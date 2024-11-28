#pragma once

#include "base.hpp"

//use the 4 macros below only when i is a constant / a single variable
#define joystk_press_const(i) __raw(0x2C,i)
#define joystk_press_val(i)   __raw(0x2D,__addr(i))
#define joystk_release_const(i) __raw(0x2E,i)
#define joystk_release_val(i)   __raw(0x2F,__addr(i))

int joystk_press(int key=__A){
    joystk_press_val(key);
}
int joystk_release(int key=__A){
    joystk_release_val(key);
}

