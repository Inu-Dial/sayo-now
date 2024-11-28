#pragma once

#include "base.hpp"

#define gk_press_const(i) __raw(0x11, i)
#define gk_press_val(i) __raw(0x15, __addr(i))
#define gk_release_const(i) __raw(0x19, i)
#define gk_release_val(i) __raw(0x1D, __addr(i))