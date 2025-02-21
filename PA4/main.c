#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//==========Macros==========
// Return Codes
#define INVALID_ARGS -1
#define GENERAL_FAILURE -2
#define OK 3
#define ENCODE_SUCCESS 0
#define DECODE_SUCCESS 1
#define FILE_ERROR -2
#define INIT_FAILURE -3
// For Arguments
#define ARG_MAX 5
#define ENCODE "encode"
#define DECODE "decode"
// MISC
#define CHAR_MAX 255
#define FREQUENCY_DEFAULT 0

//==========Structures==========
typedef struct TreeNode{
    char character;
    int frequency;
    struct TreeNode *left, *right;
}TreeNode;

typedef struct MinHeap{
    int size;
    int capacity;
    struct TreeNode ** nodes;
}MinHeap;

//==========Function Prototypes==========
// General
void printUsage();
int countFrequencies(char *, int[], int);
// Tree functions
TreeNode * createTreeNode(char, int);
// Heap functions
int initializeMinHeap(MinHeap *, int);
int deallocMinHeap(MinHeap *);
int insertMinHeap(MinHeap *, TreeNode *);
void swapHeap(MinHeap *, int, int);
void upHeap(MinHeap *, int);
void downHeap(MinHeap *, int);
int getParent(int);
int getLeftChild(int);
int getRightChild(int);

int main(int argc, char ** argv){
    char *inputTextFilePath = NULL, *codeTableFilePath = NULL, *outputFilePath = NULL;

    if(argc != ARG_MAX){
        printUsage();
        return INVALID_ARGS;
    }
    
    if(strcmp(argv[1], ENCODE) == 0){
        // File setup
        inputTextFilePath = argv[2];
        codeTableFilePath = argv[3];
        outputFilePath = argv[4];

        // Count character frequencies
        int numOfCharacters, frequencies[CHAR_MAX];
        if((numOfCharacters = countFrequencies(inputTextFilePath, frequencies, CHAR_MAX)) == FILE_ERROR){
            printf("Unable to read file: %s for frequencies.\n", inputTextFilePath);
            return FILE_ERROR;
        }

        // Setup min heap
        MinHeap minHeap;
        if(initializeMinHeap(&minHeap, numOfCharacters) == INIT_FAILURE){
            printf("Unable to initialize min heap.\n");
            return INIT_FAILURE;
        }

        if(deallocMinHeap(&minHeap) == GENERAL_FAILURE){
            printf("Unable to deallocate min heap.\n");
            return GENERAL_FAILURE;
        }
        return ENCODE_SUCCESS;
    }else if(strcmp(argv[1], DECODE) == 0){
        return DECODE_SUCCESS;
    }

    // Invalid argv[1]
    return GENERAL_FAILURE;
}

// Prints usage
void printUsage(){
    printf("Invalid arugments or not enough arguments supplied.\n");
    printf("Usage: [encode/decode] [path input text file/ path input code table file] [path output code table file/ path input encoded text file] [path output encoded text file/ path output decoded text file]\n");
}

// Returns: Number of characters read on success, FILE_ERROR on failure
int countFrequencies(char * filePath, int frequencies[], int size){
    FILE *inputFile = fopen(filePath, "r");

    if (inputFile == NULL)
        return FILE_ERROR;

    // Initialize to zero
    for(int i = 0; i < size; i++){
        frequencies[i] = FREQUENCY_DEFAULT;
    }

    char c;
    while ((c = fgetc(inputFile)) != EOF && c!='\n')
    {
       frequencies[(int)c]++;
    }

    int numOfCharacters = 0;
    for(int i = 0; i < size; i++){
        if(frequencies[i] != 0) numOfCharacters++;
    }

    fclose(inputFile);
    return numOfCharacters;
}

// Returns: OK on success, INIT_FAILURE on args failure
int initializeMinHeap(MinHeap * minHeap, int capacity){
    if(minHeap == NULL || capacity < 0) return INIT_FAILURE;

    minHeap->nodes = (TreeNode **) malloc(sizeof(TreeNode *) * capacity);

    if(minHeap->nodes == NULL) return INIT_FAILURE;

    minHeap->size = 0;
    minHeap->capacity = capacity;
    return OK;
}

// Returns OK on success, GENERAL_FAILURE on failure
int deallocMinHeap(MinHeap * minHeap){
    if(minHeap == NULL) return GENERAL_FAILURE;
    free(minHeap->nodes);
    minHeap->size = -1;
    minHeap->capacity = -1;
    return OK;
}

// Returns: The parent index given an index i
int getParent(int i){
    return (i-1)/2;
}

// Returns: The left child index given an index i
int getLeftChild(int i){
    return 2 * i + 1;
}

// Returns: The right child index given an index i
int getRightChild(int i){
    return 2 * i + 2;
}

// Returns: OK on success, GENERAL_FAILURE on failure
int insertMinHeap(MinHeap * minHeap, TreeNode * treeNode){
    if(minHeap == NULL || treeNode == NULL || minHeap->size == minHeap->capacity) return GENERAL_FAILURE;

    minHeap->nodes[minHeap->size] = treeNode;
    upHeap(minHeap, minHeap->size);
    minHeap->size++;

    return OK;
}

void swapHeap(MinHeap * minHeap, int i, int j){
    TreeNode temp = *minHeap->nodes[i];
    *minHeap->nodes[i] = *minHeap->nodes[j];
    *minHeap->nodes[j] = temp;
}

void upHeap(MinHeap * minHeap, int i){
    int parentI = (i - 1)/2;
    if(parentI < 0) return;
    
    if(minHeap->nodes[i]->frequency < minHeap->nodes[parentI]->frequency){
        swapHeap(minHeap, i, parentI);
        upHeap(minHeap, parentI);
    }

}

void downHeap(MinHeap * minHeap, int i){
    int smallestI = i;
    int leftChildI = getLeftChild(i);
    int rightChildI = getRightChild(i);

    if(leftChildI < minHeap->size && minHeap->nodes[leftChildI]->frequency < minHeap->nodes[smallestI]->frequency)
        smallestI = leftChildI;
    if(rightChildI < minHeap->size && minHeap->nodes[rightChildI]->frequency < minHeap->nodes[smallestI]->frequency)
        smallestI = rightChildI;

    if(smallestI != i){
        swapHeap(minHeap, i, smallestI);
        downHeap(minHeap, smallestI);
    }
}

// Returns TreeNode * on success, on fail, returns NULL
TreeNode * createTreeNode(char character, int frequency){
    TreeNode * newNode = (TreeNode *) malloc(sizeof(TreeNode));
    if(newNode == NULL) return NULL;
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = newNode->right = NULL;
    return newNode;
}
