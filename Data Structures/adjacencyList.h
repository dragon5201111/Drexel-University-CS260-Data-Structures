#pragma once

typedef struct Vertex Vertex;
typedef struct AdjacencyList AdjacencyList;

Vertex * createVertex(int);
AdjacencyList * createAdjacencyList(int, int);
void addEdge(AdjacencyList *, int, int);