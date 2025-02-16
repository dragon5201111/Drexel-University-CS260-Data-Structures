#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "unionFind.h"


int findUF(UnionFind * unionFind, int i){
    if(unionFind == NULL || unionFind->sets == NULL || i < 0 || i > unionFind->size){
        fprintf(stderr, "Cannot perform find. Union find or sets is null.\n");
        return INT_MIN;
    }

    if(unionFind->sets[i] != i){
        unionFind->sets[i] = findUF(unionFind, unionFind->sets[i]);
    }
    
    return unionFind->sets[i];
}

void intializeUF(UnionFind * unionFind, int size){
    size++;

    if(unionFind == NULL ||  size <= 0){
        fprintf(stderr, "Cannot intialize null union find or size of %d\n", size);
        return;
    }

    unionFind->size = size;

    if((unionFind->sets = (int *) malloc(sizeof(int) * size)) == NULL){
        fprintf(stderr, "Unable to allocate memory for sets.\n");
        return;
    }

    
    if((unionFind->setSizes = (int *) malloc(sizeof(int) * size)) == NULL){
        fprintf(stderr, "Unable to allocate memory for set sizes.\n");
        return;
    }

    for(int i = 0; i < size; i++){
        unionFind->sets[i] = i;
        unionFind->setSizes[i] = 1;
    }
}

void freeUF(UnionFind * unionFind){
    if(unionFind == NULL || unionFind->sets == NULL || unionFind->setSizes == NULL){
        fprintf(stderr, "Cannot free null union find.\n");
        return;
    }

    free(unionFind->setSizes);
    free(unionFind->sets);

    unionFind->sets = NULL;
    unionFind->setSizes = NULL;
}

void unionUF(UnionFind *unionFind, int i, int j) {
    if (unionFind == NULL || unionFind->sets == NULL || unionFind->setSizes == NULL) {
        fprintf(stderr, "Cannot perform union. Union find is not completely initialized.\n");
        return;
    }

    int parentI = findUF(unionFind, i), parentJ = findUF(unionFind, j);

    if (parentI == INT_MIN || parentJ == INT_MIN) {
        fprintf(stderr, "Unable to find parent for i or j.\n");
        return;
    } 

    if (parentI == parentJ) {
        fprintf(stderr, "Unable to create union, %d, %d, are of the same set.\n", i, j);
        return;
    }

    int parentSizeI = unionFind->setSizes[parentI];
    int parentSizeJ = unionFind->setSizes[parentJ];

    if (parentSizeJ <= parentSizeI) {
        unionFind->sets[parentJ] = parentI;
        unionFind->setSizes[parentI] += parentSizeJ;
    } else {
        unionFind->sets[parentI] = parentJ;
        unionFind->setSizes[parentJ] += parentSizeI;
    }
}

void printUF(UnionFind * unionFind){
    for(int i = 0; i < unionFind->size; i++){
        printf("%d\t", i);
    }

    putchar('\n');

    for(int i = 0; i < unionFind->size; i++){
        printf("%d\t", unionFind->sets[i]);
    }

    putchar('\n');

    printf("Sizes:\n");

    for(int i = 0; i < unionFind->size; i++){
        printf("%d\t", unionFind->setSizes[i]);
    }
    
    printf("\n\n");
}

int isOfSameSetUF(UnionFind * unionFind, int i, int j){
    if(i < 0 || j < 0 || i > unionFind->size || j > unionFind->size){
        printf("i or j out of union bounds.\n");
        return INT_MIN;
    }

    return findUF(unionFind, i) == findUF(unionFind, j);
}