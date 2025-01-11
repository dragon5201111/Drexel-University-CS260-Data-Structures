#include "common.h"
#include "stdio.h"
#include <time.h>
#include <stdlib.h>



void swap(int[], int, int);
void quickSortHelper(int[], int, int);
void quickSortN(int[], int);

int main(void){
    int arr3[] = {1,7,6,9,10};
    int arrSize3 = sizeof(arr3) / sizeof(arr3[0]);
    quickSortN(arr3, arrSize3);
    printArray(arr3, arrSize3);
}

void swap(int arr [], int i, int j){
    if(i < 0 || j < 0) return;

    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

//Pivot is first element.
void quickSortN(int arr[], int arrSize){
    quickSortHelper(arr, 0, arrSize);
}

void quickSortHelper(int arr[], int left, int right){
    if(left >= right) return;

    int pivot = arr[left];
    int i = left + 1, j = left + 1;

    int foundGTPivot = 0;

    while(j < right){
        if(arr[j] < pivot){
            swap(arr, i, j);
            i++;
        }
        j++;
    }
    swap(arr, left, i - 1); // Swap pivot
    quickSortHelper(arr, left, i - 1);
    quickSortHelper(arr, i, right);
}   