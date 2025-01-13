#include "common.h"
#include <stdio.h>


void printArray(int arr [], int arrSize){
    for (int i = 0; i < arrSize; i++)
    {
        printf(i == arrSize - 1 ? "%d" : "%d,", arr[i]);
    }
    putchar('\n');
}

void printArrayR(int arr[], int begin, int end){
    if(begin < 0 || end < 0 || end <= begin) return;

    for(; begin < end; begin++){
        printf(begin == end - 1 ? "%d" : "%d,", arr[begin]);
    }
    putchar('\n');
}

void runSortAlgAndPrint(void (*fPtr)(int[], int), int arr[], int size){
    printf("Before sorting: ");
    printArray(arr, size);
    printf("After sorting: ");
    (*fPtr)(arr, size);
    printArray(arr, size);
}