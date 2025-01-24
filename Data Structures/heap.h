#define HEAP_MAX 25000
#define M_ERR_HEAP_INIT "Cannot initialize heap, NULL heap structure or error allocating memory.\n"
#define M_ERR_HEAP_INVALID_CAP "Invalid capacity, out of range.\n"
#define M_ERR_HEAP_ALLOC -1
#define M_SUCC_HEAP_ALLOC 0

struct Heap{
    int size;
    int capacity;
    int * arr;
};

typedef struct Heap MinHeap;
typedef struct Heap MaxHeap;


// returns M_SUCC_HEAP_ALLOC on success, M_ERR_HEAP_ALLOC on fail
int initializeHeap(struct Heap *, int);

// Returns left child index.
int leftChildI(int);

// Returns right child index.
int rightChildI(int);

// Returns parent index.
int parentI(int);

void swap(int *, int *);
void insertMin(MinHeap *, int);
int extractMin(MinHeap *, int);