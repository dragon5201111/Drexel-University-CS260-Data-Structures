#pragma once

#define isInvalidCapacity(c) (c <= 0 || c >= INT_MAX)
#define isInvalidValue(v) (v <= INT_MIN || v >= INT_MAX)

#define M_INVALID_CAPACITY "Invalid capacity for heap.\n"
#define M_NULL_HEAP "Error, heap cannot be null.\n"
#define M_HEAP_ALLOC_ERROR "Unable to allocate memory for heap.\n"
#define M_HEAP_DELETE_ERROR "Unable to delete from heap at index %d\n"

#define HEAP_SUCCESS 0
#define HEAP_ERROR -1

struct Heap{
    int size;
    int capacity;
    int * items;
};

/*
    returns HEAP_ERROR on fail, HEAP_SUCCESS on success
*/
int isValidHeap(struct Heap *, int);

void initializeHeap(struct Heap *, int);

/*
    returns HEAP_ERROR on fail, HEAP_SUCCESS on success
*/
int doubleCapacityHeap(struct Heap *);

void freeHeap(struct Heap *);

/*
    returns the ith index
*/
int heapParent(int);
int heapRChild(int); 
int heapLChild(int);

void swapHeap(int *, int *);

void upHeap(struct Heap *, int);
void downHeap(struct Heap *, int);

void insertMin(struct Heap *, int);
int extractMin(struct Heap *);