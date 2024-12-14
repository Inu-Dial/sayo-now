#include <base.hpp>

int x=R0;

int fn(){
    R0=R1=0;
}

int main(){
    int y=R1;
    x=y=1;
    fn();
    // x=0, y=1 here
}