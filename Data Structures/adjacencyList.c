#include "adjacencyList.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Vertex{
    int value;
    int visited;
    Vertex * next;
} Vertex;

typedef struct AdjacencyList{
    Vertex ** vertices;
    int numVertices;
    int isDirected;
}AdjacencyList;


Vertex * createVertex(int value){
    Vertex * vertex = (Vertex *) malloc(sizeof(Vertex));

    if(vertex == NULL){
        fprintf(stderr, "Unable to allocate memory for vertex %d.\n", value);
        return NULL;
    };

    vertex->visited = N_VISITED;
    vertex->value = value;
    vertex->next = NULL;
    
    return vertex;
}

AdjacencyList * createAdjacencyList(int vertices, int isDirected){
    AdjacencyList * adjacencyList = (AdjacencyList *) malloc(sizeof(AdjacencyList));

    if(adjacencyList == NULL){
        fprintf(stderr, "Unable to allocate memory for adjacency list.\n");
        return NULL;
    }

    adjacencyList->numVertices = vertices;
    adjacencyList->isDirected = isDirected;

    adjacencyList->vertices = (Vertex **) malloc(sizeof(Vertex *) * vertices);

    if(adjacencyList->vertices == NULL){
        fprintf(stderr, "Unable to allocate memory for vertices.\n");
        return NULL;
    }

    for(int i = 0; i < vertices; i++){
        adjacencyList->vertices[i] = NULL;
    }

    return adjacencyList;
}

void addEdge(AdjacencyList * adjacencyList, int src, int dest){
    Vertex * newNode = createVertex(dest);
    // put dest in front of src list
    newNode->next = adjacencyList->vertices[src];
    adjacencyList->vertices[src] = newNode;

    if (!adjacencyList->isDirected) {
        // put src in front of dest list
        newNode = createVertex(src);
        newNode->next = adjacencyList->vertices[dest];
        adjacencyList->vertices[dest] = newNode;
    }
}

void printAdjacencyList(AdjacencyList * adjacencyList){
    for (int i = 0; i < adjacencyList->numVertices; i++)
    {   
        printf("%d->", i);
        Vertex * currentVertex = adjacencyList->vertices[i];

        while(currentVertex != NULL){
            printf("%d->", currentVertex->value);
            currentVertex = currentVertex->next;
        }
        putchar('\n');
    }
}

void freeAdjacencyList(AdjacencyList * adjacencyList){
   for (int i = 0; i < adjacencyList->numVertices; i++) {
        Vertex * currentVertex = adjacencyList->vertices[i];
        Vertex * nextVertex = NULL;

        while (currentVertex != NULL) {
            nextVertex = currentVertex->next;
            free(currentVertex);
            currentVertex = nextVertex;
        }
    }

    free(adjacencyList->vertices); 
    adjacencyList->vertices = NULL;
    free(adjacencyList);
}

int isNeighbor(AdjacencyList * adjacencyList, int src, int dest){
    Vertex * srcNode = adjacencyList->vertices[src];

    while(srcNode != NULL){
        if(srcNode->value == dest) return 1;
        srcNode = srcNode->next;
    }

    return 0;
}

void _removeFromList(AdjacencyList * adjacencyList, int src, int dest){
    Vertex * srcNode = adjacencyList->vertices[src];
    Vertex * prevNode = NULL;

    // Remove itself
    if(srcNode != NULL && srcNode->value == dest){
        adjacencyList->vertices[src] = srcNode->next;
        free(srcNode);
        return;
    }

    while (srcNode != NULL && srcNode->value != dest) {
        prevNode = srcNode;
        srcNode = srcNode->next;
    }

    // Vertex not found
    if(srcNode == NULL) return;

    prevNode->next = srcNode->next;

    free(srcNode);
}

void removeEdge(AdjacencyList * adjacencyList, int src, int dest){
    _removeFromList(adjacencyList, src, dest);

    if(!adjacencyList->isDirected){
        _removeFromList(adjacencyList, dest, src);
    }
}


void DFS(AdjacencyList * adjacencyList, int s){
    for(int i = 0; i < adjacencyList->numVertices; i++){
        Vertex * currentNode = adjacencyList->vertices[i];

        while(currentNode != NULL){
            currentNode->visited = N_VISITED;
            currentNode = currentNode->next;
        }
    }

    _DFSRec(adjacencyList, s);
}

void _DFSRec(AdjacencyList * adjacencyList, int s) {
    Vertex * current = adjacencyList->vertices[s];
    
    if (current == NULL || current->visited == VISITED) return;
    
    current->visited = VISITED;

    printf("%d ", s); 
    
    while (current != NULL) {
        if (current->visited == N_VISITED) {
            _DFSRec(adjacencyList, current->value);
        }
        current = current->next;
    }
}