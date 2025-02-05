#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "heap.h"


int isValidHeap(struct Heap * heap, int capacity){
    if(isInvalidCapacity(capacity)){
        fprintf(stderr, M_INVALID_CAPACITY);
        return HEAP_ERROR;
    }

    if(heap == NULL){
        fprintf(stderr, M_NULL_HEAP);
        return HEAP_ERROR;
    }

    return HEAP_SUCCESS;
}

int heapParent(int i){
    return (i - 1) / 2;
}

int heapLChild(int i){
    return (2*i)+1;
}

int heapRChild(int i){
    return (2*i)+2;
}

void initializeHeap(struct Heap  * heap, int capacity){
    if(isValidHeap(heap, capacity) == HEAP_ERROR) return;

    if((heap->items = (int *) malloc(sizeof(int) * capacity)) == NULL){
        fprintf(stderr, M_HEAP_ALLOC_ERROR);
        return;
    }

    heap->size = 0;
    heap->capacity =capacity;
}

int doubleCapacityHeap(struct Heap * heap){
    int * newItems = (int *) realloc(heap->items, sizeof(int) * (heap->capacity * 2));

    if(newItems == NULL){
        fprintf(stderr, M_HEAP_ALLOC_ERROR);
        return HEAP_ERROR;
    }

    heap->capacity = (heap->capacity * 2);
    heap->items = newItems;

    return HEAP_SUCCESS;
}

void freeHeap(struct Heap * heap){    
    free(heap->items);
    heap->size = 0;
}

void swapHeap(int * i, int * j){
    int temp = *i;
    *i = *j;
    *j = temp;
}

void upHeap(struct Heap * heap, int index){
    int parentIndex = heapParent(index);

    if(parentIndex < 0) return;
    
    if(heap->items[index]< heap->items[parentIndex]){
        swapHeap(&heap->items[index], &heap->items[parentIndex]);
        upHeap(heap, parentIndex);
    }
}

void insertMin(struct Heap * heap, int value){
    if(isInvalidValue(value)){
        fprintf(stderr, "Value out of range.\n");
        return;
    }
    // need to resize
    if(heap->size >= heap->capacity){
        if(doubleCapacityHeap(heap) == HEAP_ERROR) return;
    }

    heap->items[heap->size++] = value;

    upHeap(heap, heap->size - 1);
}

int extractMin(struct Heap * heap) {
    if (heap->size == 0) {
        return INT_MIN;
    }

    int minValue = heap->items[0];

    if (heap->size == 1) {
        --heap->size;
        return minValue;
    }

    swapHeap(&heap->items[0], &heap->items[--heap->size]);
    downHeap(heap, 0);
    
    return minValue;
}

void downHeap(struct Heap * heap, int parentIndex){
    int heapSize = heap->size;    
    int leftChildIndex = heapLChild(parentIndex);
    int rightChildIndex = heapRChild(parentIndex);
    int smallestIndex = parentIndex;

    if (leftChildIndex < heapSize && heap->items[leftChildIndex] < heap->items[smallestIndex]) {
        smallestIndex = leftChildIndex;
    }

    if (rightChildIndex < heapSize && heap->items[rightChildIndex] < heap->items[smallestIndex]) {
        smallestIndex = rightChildIndex;
    }

    if (smallestIndex != parentIndex) {
        swapHeap(&heap->items[parentIndex], &heap->items[smallestIndex]);
        downHeap(heap, smallestIndex);
    }
    
}

