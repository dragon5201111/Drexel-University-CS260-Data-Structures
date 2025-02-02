#include <stdio.h>
#include "heap.h"
#include "adjacencyList.h"

int main(void){
    AdjacencyList * aj = createAdjacencyList(5, N_DIRECTED);
    addEdge(aj, 0, 1);
    addEdge(aj, 0, 3);
    printAdjacencyList(aj);
    DFS(aj, 0);
    freeAdjacencyList(aj);
    return 0;
}