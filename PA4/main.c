#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//==========Macros==========
// Return Codes
#define INVALID_ARGS -1
#define GENERAL_FAILURE -2
#define OK 3
#define ENCODE_SUCCESS 0
#define DECODE_SUCCESS 0
#define FILE_ERROR -2
#define INIT_FAILURE -3
#define BUILD_FAILURE -4
#define WRITE_FAILURE -6
// For Arguments
#define ARG_MAX 5
#define ENCODE "encode"
#define DECODE "decode"
// MISC
#define CHAR_MAX 255
#define STRING_MAX 1024
#define FREQUENCY_DEFAULT 0
#define INTERNAL_CHARACTER '\0'
#define CODE_TABLE_FORMAT "%c\t%s\t%d\n"

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
int countFrequencies(char *, int[], int, int*);
void printCompressionStatistics(int, int);
int decodeCipherText(TreeNode *, char *, char *);

// Code Table functions
int initializeCodeTable(Code[], int);
int buildCodeTable(Code[], TreeNode *);
void buildCodeTableH(Code[], char *, int, TreeNode *);
void deallocCodeTable(Code[], int);
int encodePlainText(char *, char *, Code[]);
int writeCodeTable(char *, Code[], int);
int readFileIntoCodeTable(char * codeTableFilePath, Code codeTable[]);


// Tree functions
TreeNode * createTreeNode(char, int);
TreeNode * extractMin(MinHeap *);
TreeNode * buildDecodingTree(MinHeap *);
int getTreeHeight(TreeNode *);
void deallocTreeNode(TreeNode *);
int rebuildDecodingTree(TreeNode **, Code[], int);

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
        int numOfCharacters = 0, characterTotal = 0, frequencies[CHAR_MAX];
        if((numOfCharacters = countFrequencies(inputTextFilePath, frequencies, CHAR_MAX, &characterTotal)) == FILE_ERROR){
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

        // Debug to print heap
        //for(int i = 0; i < minHeap.size; i++){printf("(%c,%d),", minHeap.nodes[i]->character, minHeap.nodes[i]->frequency);}printf("\n");
        
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
        
        // Encode input file to output file
        int bits;
        if((bits = encodePlainText(inputTextFilePath, outputFilePath, codeTable)) == WRITE_FAILURE){
            printf("Unable to encode plaintext file.\n");
            deallocTreeNode(decodingTree);
            deallocMinHeap(&minHeap);
            deallocCodeTable(codeTable, CHAR_MAX);
            return WRITE_FAILURE;
        }

        // Write code table
        if(writeCodeTable(codeTableFilePath, codeTable, CHAR_MAX) == WRITE_FAILURE){
            printf("Unable to write code table to file.\n");
            deallocTreeNode(decodingTree);
            deallocMinHeap(&minHeap);
            deallocCodeTable(codeTable, CHAR_MAX);
            return WRITE_FAILURE;
        }

        printCompressionStatistics(characterTotal, bits);

        // Min heap size is zero so we need to free decoding tree as well
        // Decoding tree contains reference to all nodes (preassumably)
        deallocTreeNode(decodingTree);
        deallocMinHeap(&minHeap);
        deallocCodeTable(codeTable, CHAR_MAX);
        return ENCODE_SUCCESS;
    }else if(strcmp(argv[1], DECODE) == 0){
        //printf("Decode unimplemented!\n");
        codeTableFilePath = argv[2];
        inputTextFilePath = argv[3];
        outputFilePath = argv[4];

        Code codeTable[CHAR_MAX];
        if(initializeCodeTable(codeTable, CHAR_MAX) == INIT_FAILURE){
            printf("Unable to initialize code table.\n");
            return INIT_FAILURE;
        }
        

        if(readFileIntoCodeTable(codeTableFilePath, codeTable) == FILE_ERROR){
            printf("Unable to read %s into code table.\n", codeTableFilePath);
            deallocCodeTable(codeTable, CHAR_MAX);
            return FILE_ERROR;
        }
        // Debug for printing code table
        //for(int i = 0; i< CHAR_MAX; i++){if(codeTable[i].binaryCode != NULL){printf("Character:%c, BinaryCode:%s, Frequency:%d\n", codeTable[i].character, codeTable[i].binaryCode, codeTable[i].frequency);}}
        
        TreeNode * decodingTree = NULL;
        if(rebuildDecodingTree(&decodingTree, codeTable, CHAR_MAX) == BUILD_FAILURE){
            printf("Unable to reconstruct decoding tree.\n");
            deallocTreeNode(decodingTree);
            deallocCodeTable(codeTable, CHAR_MAX);
            return BUILD_FAILURE;
        }

        if(decodeCipherText(decodingTree, inputTextFilePath, outputFilePath) == WRITE_FAILURE){
            printf("Unable to write decoded text to %s from %s.\n", outputFilePath, inputTextFilePath);
            deallocTreeNode(decodingTree);
            deallocCodeTable(codeTable, CHAR_MAX);
            return WRITE_FAILURE;
        }

        deallocTreeNode(decodingTree);
        deallocCodeTable(codeTable, CHAR_MAX);
        return DECODE_SUCCESS;
    }

    // Invalid argv[1]
    return GENERAL_FAILURE;
}

// Returns OK on success, WRITE_FAILURE on failure
int decodeCipherText(TreeNode * decodingTree, char * inputFilePath, char * outputFilePath) {
    FILE *inputFile = fopen(inputFilePath, "r"), *outputFile = fopen(outputFilePath, "w");

    if (inputFile == NULL || outputFile == NULL) {
        return WRITE_FAILURE;
    }

    TreeNode * currentNode = decodingTree;
    char c;

    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    char *encodedData = (char *)malloc(fileSize + 1);
    if (encodedData == NULL) {
        fclose(inputFile);
        fclose(outputFile);
        return WRITE_FAILURE;
    }
    
    fread(encodedData, 1, fileSize, inputFile);
    encodedData[fileSize] = '\0';

    for (long i = 0; i < fileSize; i++) {
        c = encodedData[i];
        if (c == '0') {
            currentNode = currentNode->left;
        } else if (c == '1') {
            currentNode = currentNode->right;
        }

        if (currentNode != NULL && currentNode->left == NULL && currentNode->right == NULL) {
            fputc(currentNode->character, outputFile);
            currentNode = decodingTree;
        }
    }

    free(encodedData);
    fclose(outputFile);
    fclose(inputFile);
    return OK;
}

// Returns OK on success, BUILD_FAILURE on failure
int rebuildDecodingTree(TreeNode ** decodingTree, Code codeTable[], int size){
    *decodingTree = createTreeNode(INTERNAL_CHARACTER, FREQUENCY_DEFAULT);

    if(decodingTree == NULL) return BUILD_FAILURE;

    TreeNode * currentTreeNode;

    int j = 0;
    for(int i = 0; i < size; i++){
        if(codeTable[i].binaryCode != NULL){
            currentTreeNode = *decodingTree;
            j=0;
            while(codeTable[i].binaryCode[j]){
                if(codeTable[i].binaryCode[j] == '0') {
                    if (currentTreeNode->left == NULL) {
                        currentTreeNode->left = createTreeNode(INTERNAL_CHARACTER, FREQUENCY_DEFAULT); 
                        if(currentTreeNode->left == NULL) return BUILD_FAILURE;
                    }
                    currentTreeNode = currentTreeNode->left;
                } 
                else if(codeTable[i].binaryCode[j] == '1') {
                    if (currentTreeNode->right == NULL) {
                        currentTreeNode->right = createTreeNode(INTERNAL_CHARACTER, FREQUENCY_DEFAULT);
                        if(currentTreeNode->right == NULL) return BUILD_FAILURE;
                    }
                    currentTreeNode = currentTreeNode->right;
                }
                j++;
            }
            
            currentTreeNode->character = codeTable[i].character;
        }
    }

    return OK;
}

// Returns FILE_ERROR if file has any issues, OK otherwise.
int readFileIntoCodeTable(char * codeTableFilePath, Code codeTable[]) {
    FILE * codeTableFile = fopen(codeTableFilePath, "r");
    if(codeTableFile == NULL) {
        return FILE_ERROR;
    }

    char lineBuffer[STRING_MAX];
    char character;
    char frequency[STRING_MAX];
    char code[STRING_MAX];

    while(fgets(lineBuffer, sizeof(lineBuffer), codeTableFile)) {
        if (lineBuffer[0] == '\0' || lineBuffer[0] == '\n') continue;

        character = lineBuffer[0];

        int i = 1;
        while(i < STRING_MAX && isspace(lineBuffer[i])) i++;

        int j = 0;
        while(i < STRING_MAX && !isspace(lineBuffer[i])) code[j++] = lineBuffer[i++];
        code[j] = '\0';
        i++;

        j = 0;
        while(i < STRING_MAX && isdigit(lineBuffer[i])) frequency[j++] = lineBuffer[i++];
        frequency[j] = '\0';

        codeTable[(int)character].binaryCode = strdup(code);
        if (codeTable[(int)character].binaryCode == NULL) {
            fclose(codeTableFile);
            return FILE_ERROR;
        }

        codeTable[(int)character].frequency = atoi(frequency);
        if(codeTable[(int)character].frequency <= 0) {
            fclose(codeTableFile);
            return FILE_ERROR;
        }

        codeTable[(int)character].character = character;
    }

    fclose(codeTableFile);
    return OK;
}

// Prints compression statistics
void printCompressionStatistics(int numOfCharacters, int bits){
    printf("Original: %d bits\n", numOfCharacters*8); //assuming that you store the number of characters in variable "uncompressed". *8 is because ASCII table uses 8 bits to represent each character
    printf("Compressed: %d bits\n", bits); //assuming that you store the number of bits (i.e., 0/1s) of encoded text in variable "compressed_size"
    printf("Compression Ratio: %.2f%%\n", (float)bits/((float)numOfCharacters*8)*100); //This line will print the compression ration in percentages, up to 2 decimals.
}

// Prints usage
void printUsage(){
    printf("Invalid arugments or not enough arguments supplied.\n");
    printf("Usage: [encode/decode] [path input text file/ path input code table file] [path output code table file/ path input encoded text file] [path output encoded text file/ path output decoded text file]\n");
}

// Returns: Bits wrote on success (characters), WRITE_FAILURE on failure to write or open file
int encodePlainText(char * inputFilePath, char * outputFilePath, Code codeTable[]){
    FILE *inputFile = fopen(inputFilePath, "r"), *outputFile = fopen(outputFilePath, "w");

    if (inputFile == NULL || outputFile == NULL)
    {
     return WRITE_FAILURE;
    }

    char c;
    char * binaryCode;
    int bits = 0;
    while ((c = fgetc(inputFile)) != EOF && c!='\n')
    {
        binaryCode = codeTable[(int)c].binaryCode;
        bits += strlen(binaryCode);
       fprintf(outputFile, "%s", binaryCode);
    }

    fclose(outputFile);
    fclose(inputFile);
    return bits;
}

// Returns: OK on write success, WRITE_FAILURE on failure to write
int writeCodeTable(char * filePath, Code codeTable [], int size){
    FILE *codeTableFile = fopen(filePath, "w");
    if (codeTableFile == NULL)
    {
     return WRITE_FAILURE;
    }

    for(int i = size-1; i >= 0; i--)
    { 
      if(codeTable[i].binaryCode != NULL)
        fprintf(codeTableFile, CODE_TABLE_FORMAT, codeTable[i].character, codeTable[i].binaryCode, codeTable[i].frequency);
    }
    fclose(codeTableFile);
    return OK;
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
int countFrequencies(char * filePath, int frequencies[], int size, int * characterTotal){
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
       *characterTotal += 1;
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

void upHeap(MinHeap *minHeap, int i) {
    int parentI = getParent(i);
    if (parentI < 0) return;  // If we reach the root, stop recursion.

    // Compare frequencies, and if tied, compare ASCII values
    if (minHeap->nodes[i]->frequency < minHeap->nodes[parentI]->frequency ||
        (minHeap->nodes[i]->frequency == minHeap->nodes[parentI]->frequency &&
         minHeap->nodes[i]->character < minHeap->nodes[parentI]->character)) {
        
        // Swap nodes
        swapHeap(minHeap, i, parentI);

        // Recursively call upHeap for the parent index
        upHeap(minHeap, parentI);
    }
}

void downHeap(MinHeap *minHeap, int i) {
    int smallestI = i;
    int leftChildI = getLeftChild(i);
    int rightChildI = getRightChild(i);
    
    if (leftChildI < minHeap->size) {
        if (minHeap->nodes[leftChildI]->frequency < minHeap->nodes[smallestI]->frequency ||
            (minHeap->nodes[leftChildI]->frequency == minHeap->nodes[smallestI]->frequency &&
             minHeap->nodes[leftChildI]->character < minHeap->nodes[smallestI]->character)) {
            smallestI = leftChildI;
        }
    }

    if (rightChildI < minHeap->size) {
        if (minHeap->nodes[rightChildI]->frequency < minHeap->nodes[smallestI]->frequency ||
            (minHeap->nodes[rightChildI]->frequency == minHeap->nodes[smallestI]->frequency &&
             minHeap->nodes[rightChildI]->character < minHeap->nodes[smallestI]->character)) {
            smallestI = rightChildI;
        }
    }
    if (smallestI != i) {
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