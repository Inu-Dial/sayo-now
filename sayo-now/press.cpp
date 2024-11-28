#include <base.hpp>
#include <keyboard.hpp>
#include <led.hpp>
#include <sleep.hpp>

int main(){
    enter_jog_mode();
    select_led(3);
    set_led_color(0xff,0x00,0x00); // red
    while(!!!IO);
    R0=0;
    if(1!=R0){
        set_led_color(0x00,0x00,0xff); // blue
        sleep(1000);
    }
    set_led_color(0x00,0xff,0x00); // green
}
