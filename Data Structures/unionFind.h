typedef struct UnionFind{
    int * sets;
    int * setSizes;
    int size;
}UnionFind;

void intializeUF(UnionFind *, int);
void freeUF(UnionFind *);
int findUF(UnionFind *, int);
void unionUF(UnionFind *, int, int);
void printUF(UnionFind *);
int isOfSameSetUF(UnionFind *, int, int);