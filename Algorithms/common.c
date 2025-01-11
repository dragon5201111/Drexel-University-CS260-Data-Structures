#include "common.h"
#include <stdio.h>


void printArray(int arr [], int arrSize){
    for (int i = 0; i < arrSize; i++)
    {
        printf(i == arrSize - 1 ? "%d" : "%d,", arr[i]);
    }
    putchar('\n');
}
