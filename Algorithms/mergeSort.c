#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

void mergeSort(int [], int);
void mergeArr(int [], int [], int [], int);

int main(void){
    int arr[] = {6,5,1,4,6,23,4,5,6,7,90, 14};
    int arrSize = sizeof(arr)/sizeof(int);
    runSortAlgAndPrint(&mergeSort, arr, arrSize);
    return 0;
}

void mergeSort(int arr[], int arrSize){
    if(arrSize <= 1) return;

    int leftSize = arrSize / 2;
    int rightSize = arrSize - leftSize;

    int leftArr[leftSize];
    int rightArr[rightSize];

    int i = 0, j = 0;

    while(i < arrSize){
        if(i < leftSize){
            leftArr[i] = arr[i];
        }else{
            rightArr[j] = arr[i];
            j++;
        }
        i++;
    }

    mergeSort(leftArr, leftSize);
    mergeSort(rightArr, rightSize);
    /*
    putchar('\n');
    printf("After sort:\n");
    printf("Right:");
    printArray(rightArr, rightSize);
    printf("Left:");
    printArray(leftArr, leftSize);
    printf("Orig:");
    printArray(arr, arrSize);
    */
    mergeArr(leftArr, rightArr, arr, arrSize);
    /*
    printf("After merge:");
    printArray(arr, arrSize);
    */
}

void mergeArr(int leftArr [], int rightArr [],int arr [], int arrSize){
    int leftSize = arrSize / 2;
    int rightSize = arrSize - leftSize;

    int i = 0, l = 0, r = 0;

    while(l < leftSize && r < rightSize){
        if(leftArr[l] < rightArr[r]){
            arr[i] = leftArr[l++];
        }else{
            arr[i] = rightArr[r++];
        }
        i++;
    }

    while (l < leftSize)
    {
        arr[i++] = leftArr[l++];
    }
    
    while (r < rightSize)
    {
        arr[i++] = rightArr[r++];
    }

}
