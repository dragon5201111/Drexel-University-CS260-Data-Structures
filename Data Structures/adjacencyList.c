#include "adjacencyList.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Vertex{
    int value;
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
  

}