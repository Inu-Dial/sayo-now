#pragma once

#include "base.hpp"

int __selected_led = 0x1B;
int __selected_led_color = 0x1C;
int __all_led_color = 0x1D;

#define color_red     0xff,0x00,0x00
#define color_green   0x00,0xff,0x00
#define color_blue    0x00,0x00,0xff
#define select_led(led_id)          (__selected_led=(led_id))
#define set_all_led_color(r,g,b)    (__all_led_color=(r)|((g)<<8)|((b)<<16))
#define set_led_color(r,g,b)        (__selected_led_color=(r)|((g)<<8)|((b)<<16))