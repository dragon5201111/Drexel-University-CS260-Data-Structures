#pragma once


enum AdjacencyType {DIRECTED=1, N_DIRECTED=0};
enum VertexConstants {VISITED=1, N_VISITED=0};

typedef struct Vertex Vertex;
typedef struct AdjacencyList AdjacencyList;

Vertex * createVertex(int);
AdjacencyList * createAdjacencyList(int, int);
void addEdge(AdjacencyList *, int, int);
void _removeFromList(AdjacencyList *, int, int);
void removeEdge(AdjacencyList *, int, int);
void printAdjacencyList(AdjacencyList *);
void freeAdjacencyList(AdjacencyList *);
int isNeighbor(AdjacencyList *,int, int);

void DFS(AdjacencyList *, int);
void _DFSRec(AdjacencyList *, int);
