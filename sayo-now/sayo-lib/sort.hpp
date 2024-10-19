#pragma once

#include "prog.hpp"
#include "Rx.hpp"
#include "array.hpp"

#define cur R9
#define low R10
#define pivot R11
int partition_32(int begin = R7, int high = R8){
    low=0;
    --high;
    pivot=P32_R7;
    while (low < high) {
        get_arr32_Rx(R7,high,cur);
        while (low < high && cur >= pivot) {
            --high;
            get_arr32_Rx(R7,high,cur);
        }
        get_arr32_Rx(R7,high,cur);
        set_arr32_Rx(R7,low,cur);
        // A[low] = A[high];
        get_arr32_Rx(R7,low,cur);
        while (low < high && cur <= pivot) {
            ++low;
            get_arr32_Rx(R7,low,cur);
        }
        get_arr32_Rx(R7,low,cur);
        set_arr32_Rx(R7,high,cur);
    }
    set_arr32_Rx(R7,low,pivot);
    return low;
}
int qsort_32(int begin = R7, int size = R11){
    if(!size) return;
    pivot=partition_32(begin,size);
    push(pivot);
    qsort_32(begin,pivot);
    pop(pivot);
    qsort_32(begin+(pivot+1 << 2),size-pivot-1);
}
#undef cur
#undef low
#undef pivot
