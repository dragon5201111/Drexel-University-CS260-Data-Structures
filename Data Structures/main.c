#include <stdio.h>
#include "heap.h"
#include "adjacencyList.h"

int main(void){
    struct Heap heap;
    initializeHeap(&heap, 50);
    insertMin(&heap, 30);
    insertMin(&heap, 2);
    insertMin(&heap, 5);
    insertMin(&heap, 1);
    insertMin(&heap, 4);
    extractMin(&heap);

    
    for(int i = 0; i < heap.size; i++)
        printf("%d,", heap.items[i]);
    freeHeap(&heap);
    return 0;
}