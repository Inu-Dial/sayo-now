#ifndef __PRE_DEFINED_ADDRESS
#define __PRE_DEFINED_ADDRESS

#ifndef SAYO_NOW    // to make program valid in c++ highlight

#define __addr(x) 0
#define __var(x) *((void*)x)
#define __raw(...) 0

#endif

// __A and __B must be defined to specify address of A and B
int __A = 16;
int __B = 17;

// sorry for only one variable a time ;-;
// ARG0~4: script argument
int ARG0    = 0;
int ARG1    = 1;
int ARG2    = 2;
int ARG3    = 3;
int R0      = 4;
int R1      = 5;
int R2      = 6;
int R3      = 7;
int P_R0    = 11;
int P_R1    = 12;
int P_R2    = 13;
int P_R3    = 14;
int DPTR    = 9;
int P_DPTR  = 8;
int IO      = 10;

int __NONE__ = 15;

int __SYS_TIME_MS   = 18;
int __SYM_TIME_S    = 19;
int __SYS_KEY_LAY   = 22;
int __SCRIPT_ADDR   = 23;

#define exit() __raw(0xFF)
#define __enter_jog_mode() __raw(0xF8)
#define __current_key_layer __var(7)
#define __nop() __raw(0x01)

#define endl 10
#define __print_ascii(c) __raw(0x30,c)
#define __print_uint_const(c) __raw(0x31,c)
#define __print_ascii_rand() __raw(0x32)
#define __print_uint_val(reg) __raw(0x33,reg)

#define __color_red     0xff,0x00,0x00
#define __color_green   0x00,0xff,0x00
#define __color_blue    0x00,0x00,0xff
#define __set_led_color_const(r,g,b)    __raw(0xE1,r,g,b)
// #define __set_led_color_variable(r,g,b) __raw(???,__addr(r),__addr(g),__addr(b))
#define __select_led(c) __raw(0xE0,c)
#define __turn_on_led() __raw(0xE0)
#define __turn_off_led() __raw(0xE0,128)

#define __sleep_const(ms) {__raw(0x05,ms/256);__raw(0x06,ms%256);}
#define __sleep_rand_const(ms) { \
    if(ms/256) __raw(0x07,ms/256);  \
    if(ms%256) __raw(0x08,ms%256);  \
}

#define __swap(x,y) __raw(0x6A,__addr(x),__addr(y))

#define get_array(Rx,i,tar) { \
    Rx+=i;      \
    tar=P_##Rx; \
    Rx-=i;      \
}
#define set_array(Rx,i,num) {\
    Rx+=i;      \
    P_##Rx=num; \
    Rx-=i;      \
}

#endif