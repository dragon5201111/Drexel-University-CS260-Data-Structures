#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int main(void){
    return 0;
}

void initializeHeap(struct Heap * heap, int capacity){
    if(capacity < 1 || capacity > HEAP_MAX){
        printf(M_ERR_HEAP_INVALID_CAP);
        return;
    }

    if(heap == NULL){
        fprintf(stderr, M_ERR_HEAP_INIT);
        return;
    }

    heap->size = 0;
    heap->capacity = capacity;
    heap->arr = (int *) malloc(sizeof(int) * capacity);

    if(heap->arr == NULL){
        printf(M_ERR_HEAP_INIT);
        return;
    }
}

int leftChildI(int i){
    return 2 * i + 1;
}

int rightChildI(int i){
    return 2 * i + 2;
}

int parentI(int i){
    // floor not needed, truncated toward zero automatically
    return (i - 1)/2;
}

void swap(int *i, int *j){
    int temp = *i;
    *i = *j;
    *j = temp;
}