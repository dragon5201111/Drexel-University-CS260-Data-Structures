#include "common.h"
#include "stdio.h"

void swap(int[], int, int);
void quickSortHelperN(int[], int, int);
void qSortCodeTableH(int [], int, int);
void quickSortN(int[], int);
void qSortCodeTable(int[], int);

int main(void){
    int arr3[] = {1,7,3,2,5,6,7,8};
    int arrSize3 = sizeof(arr3) / sizeof(arr3[0]);
    runSortAlgAndPrint(&qSortCodeTable, arr3, arrSize3);
}


//Pivot is first element; N is for naive implementation
void quickSortN(int arr[], int arrSize){
    quickSortHelperN(arr, 0, arrSize);
}

//Pivot is middle element
void qSortCodeTable(int arr[], int arrSize){
    qSortCodeTableH(arr, 0, arrSize);
}

void qSortCodeTableH(int arr[], int left, int right) {
    if (left >= right) return;

    int midIndx = (left + right) / 2;
    int i = left;
    int j = right;
    int pivot = arr[midIndx];

    while (i <= j) {
        while (arr[i] < pivot) i++;

        while (arr[j] > pivot) j--;

        if (i <= j) {
            swap(arr, i, j);
            i++;
            j--;
        }
    }

    qSortCodeTableH(arr, left, j);
    qSortCodeTableH(arr, i, right);
}


void quickSortHelperN(int arr[], int left, int right){
    if(left >= right) return;

    int pivot = arr[left];
    int i = left + 1, j = left + 1;

    while(j < right){
        if(arr[j] < pivot){
            swap(arr, i, j);
            i++;
        }
        j++;
    }
    swap(arr, left, i - 1); // Swap pivot
    quickSortHelperN(arr, left, i - 1);
    quickSortHelperN(arr, i, right);
}   