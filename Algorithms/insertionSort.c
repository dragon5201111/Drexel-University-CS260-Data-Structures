#include <stdio.h>
#include "common.h"


void insertionSort(int [], int);

int main(void){
    int arr[] = {3,5,6,1,2,5};
    runSortAlgAndPrint(&insertionSort, arr, 6);
    return 0;
}

void insertionSort(int arr [], int size){
    if(size <= 0) return;
    
    for(int i = 1; i < size; i++){
        int current = arr[i];
        int j = i - 1;


        for(; arr[j] > current && j >= 0; j--){
            arr[j + 1] = arr[j];
        }

        arr[j + 1] = current;
    }
}
