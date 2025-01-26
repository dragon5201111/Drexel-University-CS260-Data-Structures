#include <stdio.h>
#include "heap.h"

int main(void){
    struct Heap h;
    int capacity = 10;
    initializeHeap(&h, capacity);
    freeHeap(&h, capacity);
    return 0;
}