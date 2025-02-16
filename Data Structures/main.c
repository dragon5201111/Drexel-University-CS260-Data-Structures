#include <stdio.h>
#include "heap.h"
#include "adjacencyList.h"
#include "unionFind.h"

int main(void){
    UnionFind uf;
    intializeUF(&uf, 5);

    printUF(&uf);
    unionUF(&uf, 0, 1);
    unionUF(&uf, 3, 4);
    unionUF(&uf, 0, 3);
    printUF(&uf);
    printf("Is of same set %d\n", isOfSameSetUF(&uf, 3, 4));
    freeUF(&uf);
    return 0;
}