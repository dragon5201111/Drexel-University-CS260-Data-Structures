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
#define BUILD_FAILURE -4
// For Arguments
#define ARG_MAX 5
#define ENCODE "encode"
#define DECODE "decode"
// MISC
#define CHAR_MAX 255
#define FREQUENCY_DEFAULT 0
#define INTERNAL_CHARACTER '\0'

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

typedef struct Code{
    char character;
    int frequency;
    char * binaryCode;
}Code;

//==========Function Prototypes==========
// General
void printUsage();
int countFrequencies(char *, int[], int);

// Code Table functions
int initializeCodeTable(Code[], int);
int buildCodeTable(Code[], TreeNode *);
void buildCodeTableH(Code[], char *, int, TreeNode *);
void deallocCodeTable(Code[], int);


// Tree functions
TreeNode * createTreeNode(char, int);
TreeNode * extractMin(MinHeap *);
TreeNode * buildDecodingTree(MinHeap *);
int getTreeHeight(TreeNode *);
void deallocTreeNode(TreeNode *);

// Heap functions
int initializeMinHeap(MinHeap *, int);
int buildMinHeap(MinHeap *, int[], int, int);
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
        if(initializeMinHeap(&minHeap, CHAR_MAX) == INIT_FAILURE){
            printf("Unable to initialize min heap.\n");
            return INIT_FAILURE;
        }

        if(buildMinHeap(&minHeap, frequencies, numOfCharacters, CHAR_MAX) == BUILD_FAILURE){
            printf("Unable to build min heap.\n");
            deallocMinHeap(&minHeap);
            return BUILD_FAILURE;
        }
        
        // Debug to test extract min
        //TreeNode * min; while((min = extractMin(&minHeap))) printf("Extracted: '%c':%d\n", min->character, min->frequency);

        // Build decoding tree
        TreeNode * decodingTree;
        if((decodingTree = buildDecodingTree(&minHeap)) == NULL){
            printf("Unable to build decoding tree.\n");
            deallocMinHeap(&minHeap);
            return BUILD_FAILURE;
        }

        //Initialize code table
        Code codeTable[CHAR_MAX];
        if(initializeCodeTable(codeTable, CHAR_MAX) == INIT_FAILURE){
            printf("Unable to initialize code table.\n");
            deallocTreeNode(decodingTree);
            deallocMinHeap(&minHeap);
            return INIT_FAILURE;
        }

        // Build code table
        if(buildCodeTable(codeTable, decodingTree) == BUILD_FAILURE){
            printf("Unable to build code table.\n");
            deallocCodeTable(codeTable, CHAR_MAX);
            deallocTreeNode(decodingTree);
            deallocMinHeap(&minHeap);
            return BUILD_FAILURE;
        }

        // Debug to print code table
        //for(int i = 0; i < CHAR_MAX; i++) {if(codeTable[i].frequency != FREQUENCY_DEFAULT) printf("Character:%c, Frequency: %d, Binary Code:%s\n", codeTable[i].character, codeTable[i].frequency, codeTable[i].binaryCode);}
        
        // Min heap size is zero so we need to free decoding tree as well
        // Decoding tree contains reference to all nodes (preassumably)
        deallocTreeNode(decodingTree);
        deallocMinHeap(&minHeap);
        deallocCodeTable(codeTable, CHAR_MAX);
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

void deallocCodeTable(Code codeTable[], int size){
    if(codeTable == NULL) return;

    for(int i = 0; i < size; i++){
        if(codeTable[i].binaryCode != NULL) free(codeTable[i].binaryCode);
    }
}

// Returns: OK on build success, BUILD_FAILURE on build failure.
int buildCodeTable(Code codeTable[], TreeNode * decodingTree){
    if(codeTable == NULL || decodingTree == NULL) return BUILD_FAILURE;
    // Height of tree plus 1 for null terminator
    char code[getTreeHeight(decodingTree) + 1];
    buildCodeTableH(codeTable, code, 0, decodingTree);
    return OK;
}

// Helper function to build code table
void buildCodeTableH(Code codeTable[], char * code, int codeLevel, TreeNode * decodingTree){
    if(decodingTree == NULL) return;
    
    // Not internal node and is a leaf node
    if(decodingTree->character != INTERNAL_CHARACTER && (decodingTree->left == NULL && decodingTree->right == NULL)){
        codeTable[(int)decodingTree->character].character = decodingTree->character;
        codeTable[(int)decodingTree->character].frequency = decodingTree->frequency;

        // Copy code
        code[codeLevel] = '\0';
        codeTable[(int)decodingTree->character].binaryCode = strdup(code);
    }

    code[codeLevel] = '0';
    buildCodeTableH(codeTable, code, codeLevel + 1, decodingTree->left);
    code[codeLevel] = '1';
    buildCodeTableH(codeTable, code, codeLevel + 1, decodingTree->right);
}

// Returns: OK on success, INIT_FAILURE on failure
int initializeCodeTable(Code codeTable[], int size){
    if(codeTable == NULL || size < 0) return INIT_FAILURE;

    for(int i = 0; i < size; i++){
        codeTable[i].frequency = FREQUENCY_DEFAULT;
        codeTable[i].binaryCode = NULL;
    }
    
    return OK;
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

// Returns: OK on success, BUILD_FAILURE on failure
int buildMinHeap(MinHeap * minHeap, int frequencies[], int numOfCharacters, int frequencySize){
    if(minHeap == NULL || numOfCharacters < 0 || frequencies == NULL) return BUILD_FAILURE;

    int frequency = 0;

    for(int i = 0; i < frequencySize && minHeap->size != numOfCharacters; i++){
        if((frequency = frequencies[i]) != FREQUENCY_DEFAULT){
            TreeNode * newNode = createTreeNode((char)i, frequency);
            if(newNode == NULL){
                return BUILD_FAILURE;
            }
            minHeap->nodes[minHeap->size++] = newNode;
        }
    }

    // Restore heap property, by min heapifying
    for (int i = (minHeap->size - 1) / 2; i >= 0; --i) 
        downHeap(minHeap, i); 
    return OK;
}

// Returns OK on success, GENERAL_FAILURE on failure
int deallocMinHeap(MinHeap * minHeap){
    if(minHeap == NULL) return GENERAL_FAILURE;

    // Can't use capacity or else seg fault
    for(int i = 0; i < minHeap->size; i++){
        deallocTreeNode(minHeap->nodes[i]);
    }
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

    minHeap->nodes[minHeap->size++] = treeNode;
    upHeap(minHeap, minHeap->size-1);
    return OK;
}

void swapHeap(MinHeap * minHeap, int i, int j){
    TreeNode * tempTreeNode = minHeap->nodes[i];
    minHeap->nodes[i] = minHeap->nodes[j];
    minHeap->nodes[j] = tempTreeNode;
}

void upHeap(MinHeap * minHeap, int i){
    int parentI = getParent(i);
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

// Returns: TreeNode * on success, on fail, returns NULL
TreeNode * createTreeNode(char character, int frequency){
    TreeNode * newNode = (TreeNode *) malloc(sizeof(TreeNode));
    if(newNode == NULL) return NULL;
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Returns: TreeNode * on success, on fail, returns NULL 
TreeNode * extractMin(MinHeap * minHeap){
    if(minHeap == NULL || minHeap->size == 0) return NULL;

    TreeNode * minTreeNode = minHeap->nodes[0];

    // Don't bother downheaping for one node
    if(minHeap->size == 1){
        minHeap->size--;
        return minTreeNode;
    }

    // Decrement size first
    swapHeap(minHeap, 0, --minHeap->size);
    // Restore heap property
    downHeap(minHeap, 0);
    return minTreeNode;
}

// Returns: NULL on failure, otherwise TreeNode *
TreeNode * buildDecodingTree(MinHeap * minHeap){
    if(minHeap == NULL) return NULL;

    TreeNode * leftTreeNode, *rightTreeNode, *insertTreeNode;
    int sizeOfHeap = minHeap->size;

    while(minHeap->size != 1){
        leftTreeNode = extractMin(minHeap);
        rightTreeNode = extractMin(minHeap);

        insertTreeNode = createTreeNode(INTERNAL_CHARACTER, leftTreeNode->frequency + rightTreeNode->frequency);

        if(insertTreeNode == NULL){
            // Heap size is decremented from extractMin so size must be restored to properly free all nodes.
            minHeap->size = sizeOfHeap;
            return NULL;
        }

        insertTreeNode->left = leftTreeNode;
        insertTreeNode->right = rightTreeNode;

        insertMinHeap(minHeap, insertTreeNode);
    }

    return extractMin(minHeap);
}

void deallocTreeNode(TreeNode * treeNode){
    if(treeNode == NULL) return;

    deallocTreeNode(treeNode->left);
    deallocTreeNode(treeNode->right);

    free(treeNode);
}

// Returns: Height of tree
int getTreeHeight(TreeNode * treeNode){
    if(treeNode == NULL) return 0;

    int leftHeight = getTreeHeight(treeNode->left);
    int rightHeight = getTreeHeight(treeNode->right);

    return ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
}