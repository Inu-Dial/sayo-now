#include "sayo-lib/base.hpp"
#include "sayo-lib/Rx.hpp"
#include "sayo-lib/array.hpp"
int x=4;            // assign x with address 4
int xx=x;           // assign xx with address of x
                    // same as "int xx=__addr(x);"
int y=5;    
int func(int y=6);  //function declaration
int func2(int x=ARG0);  //reuse ARG0, see below for reasons
int A0=ARG0;
int func3(int x=A0);    //ARG0 -> A0, shorter, maybe easier
int func4();

// the entrance of the program
// Assign the addresses of ARGx to Vx, 
// such that their values will be pushed into stack to avoid being changed
// (after every function call, they will be recovered from the stack)
// and for functions with no more than 4, AND 8-BIT ARGUMENTS,
//      you can reuse the addresses of ARGx (ARGx is an 8-bit variable)
// It's recommended to do so for every program,
//      unless you know what you are doing!
// However, Vx will be pushed into stack every function call (like func and func3 in this case)
//      and make code size larger
//      so only write down Vx you really use
int main(int V1=ARG1, int V0=ARG0){   //order doesn't matter
    for(V0=0;V0<=8;++V0){
        ++V1;
        continue;
        ++V1;
        break;
        ++V1;
    }
    while(1)++V1;
    while(0) ++V1;
    while(V0){++V1;}
    do {++V1;} while(1);
    do {++V1;} while (0);
    do {++V1;} while (V0);

    x=func(123+123*2/4)*456;  
    x+=V0+V1;
    x=func(112)?func(113):func(114);
    x=(x<10);
    y=func3(x*70)+80;
    swap(x,y);                // as shown in prog.hpp
    func4();
    return x||y;
}
int func(int x=6){  // function definition: symbol may have different names, 
                    // but no different addressed
    int y=7;
    return x*3;
}
int func2(int x=ARG0){
    return x;
}

#define MOV_ASM_CODE 0x6e
int func3(int x=ARG0){  //only need to make sure addresses don't conflict,
                        //defined argument names may be different from declared
    __raw(MOV_ASM_CODE, __addr(x), 0);
    return func2(x);
}
// you may have noticed that sayo-now doesn't 
// optimize code size well, so you can just use
// __raw(command[,arg0[,arg1...]]) to insert a raw
// asm code command [arg0 [arg1]] ... like shown above
// command arg0 arg1 ... should be int constants


int func4(){
    //do nothing
    //return will be added automatically
}
int MOV8(int ptr=ARG0){
    R0=ptr;
    P8_R0=114514;
}