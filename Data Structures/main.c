#include <stdio.h>
#include "heap.h"

int main(void){
    struct Heap h;
    int capacity = 10;
    initializeHeap(&h, capacity);
    insertMin(&h, 4);
    insertMin(&h, 7);
    insertMin(&h, 8);
    insertMin(&h, 3);
    freeHeap(&h, capacity);
    return 0;
}