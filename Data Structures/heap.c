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

int doubleCapacityHeap(struct Heap * heap, int capacity){
    if(isValidHeap(heap, capacity) == HEAP_ERROR) return HEAP_ERROR;

    int * newItems = (int *) realloc(heap->items, sizeof(int) * (capacity * 2));

    if(newItems == NULL){
        fprintf(stderr, M_HEAP_ALLOC_ERROR);
        return HEAP_ERROR;
    }

    heap->capacity = (capacity * 2);
    heap->items = newItems;

    return HEAP_SUCCESS;
}

void freeHeap(struct Heap * heap, int capacity){
    if(isValidHeap(heap, capacity) == HEAP_ERROR) return;
    
    free(heap->items);
    heap->size = 0;
}

void swapHeap(int * i, int * j){
    int temp = *i;
    *i = *j;
    *j = temp;
}

void insertMin(struct Heap * heap, int value, int capacity){
    if(isInvalidValue(value)){
        fprintf(stderr, "Value out of range.\n");
        return;
    }

    if(isValidHeap(heap, capacity) == HEAP_ERROR) return;

    // need to resize
    if((heap->size + 1) == heap->capacity){
        if(doubleCapacityHeap(heap, capacity) == HEAP_ERROR) return;
    }

    heap->items[heap->size++] = value;

    int currentIndex= heap->size - 1;
    int parentIndex = heapParent(currentIndex);
    
    while(currentIndex > 0 && heap->items[parentIndex] > heap->items[currentIndex]){
        swapHeap(&heap->items[parentIndex], &heap->items[currentIndex]);
        currentIndex = parentIndex;
        parentIndex = heapParent(parentIndex);
    }
   
}

int extractMin(struct Heap * heap, int capacity) {
    if (isValidHeap(heap, capacity) == HEAP_ERROR) {
        return -1;
    }

    if (heap->size == 0) {
        return -1;
    }

    int minValue = heap->items[0];

    if (heap->size == 1) {
        --heap->size;
        return minValue;
    }

    swapHeap(&heap->items[0], &heap->items[--heap->size]);

    int currentIndex = 0;
    int heapSize = heap->size;

    while (currentIndex < heapSize) {
        int leftIndex = heapLChild(currentIndex);
        int rightIndex = heapRChild(currentIndex);
        int smallestIndex = currentIndex;

        if (leftIndex < heapSize && heap->items[leftIndex] < heap->items[smallestIndex]) {
            smallestIndex = leftIndex;
        }

        if (rightIndex < heapSize && heap->items[rightIndex] < heap->items[smallestIndex]) {
            smallestIndex = rightIndex;
        }

        if (smallestIndex != currentIndex) {
            swapHeap(&heap->items[currentIndex], &heap->items[smallestIndex]);
            currentIndex = smallestIndex;
        } else {
            break;
        }
    }

    return minValue;
}
