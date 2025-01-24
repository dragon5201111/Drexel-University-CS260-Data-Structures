#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int main(void){
    return 0;
}

int initializeHeap(struct Heap * heap, int capacity){
    if(capacity < 1 || capacity > HEAP_MAX){
        printf(M_ERR_HEAP_INVALID_CAP);
        return M_ERR_HEAP_ALLOC;
    }

    if(heap == NULL){
        fprintf(stderr, M_ERR_HEAP_INIT);
        return M_ERR_HEAP_ALLOC;
    }

    heap->size = 0;
    heap->capacity = capacity;
    heap->arr = (int *) malloc(sizeof(int) * capacity);

    if(heap->arr == NULL){
        printf(M_ERR_HEAP_INIT);
        return M_ERR_HEAP_ALLOC;
    }

    return M_SUCC_HEAP_ALLOC;
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