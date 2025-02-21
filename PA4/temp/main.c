#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for strcmp
#define ENCODE "encode"
#define DECODE "decode"

// Return code constants
#define ENCODE_SUCCESS 0 // On encoding success
#define ENCODE_FAILURE 1 // On encoding failure
#define DECODE_SUCCESS 2 // On decoding success
#define DECODE_FAILURE 3 // On decoding failure
#define GENERAL_FAILURE 4 // On incorrect usage, i.e., not enough arguments supplied, or argv[1] was not encode or decode

// General constants
#define CHAR_MAX 256
#define ARG_MIN 5
#define INTERNAL_NODE_CHARACTER '\0'

// a struct to keep frequency and binary code representation of a character
typedef struct code{
  char character;
  unsigned int freq;
  char *binaryCode;
} Code;

typedef struct MinHeapNode{
  char character;
  int freq;
  struct MinHeapNode *left, *right;
}MinHeapNode;

typedef struct MinHeap{
  int size;
  int capacity;
  struct MinHeapNode ** nodes;
}MinHeap;

// Heap prototypes
MinHeapNode* createNode(char, int);
MinHeap * buildMinHeap(Code[], int);
MinHeap * createMinHeap(int);
MinHeapNode* extractMin(MinHeap*);
MinHeapNode* buildHuffmanTree(MinHeap *);
void downHeap(MinHeap*, int);
int minHeapLChild(int);
int minHeapRChild(int);
int minHeapParent(int);
void insertMinHeap(MinHeap*,MinHeapNode*); 
void minHeapify(MinHeap*);
void freeMinHeap(MinHeap *);
void freeNode(MinHeapNode *);
void swapMinHeap(MinHeapNode **, MinHeapNode **);
int generateHuffmanCodes(MinHeapNode *, Code [], Code []);
void generateHuffmanCodesH(MinHeapNode*, char *, int, int*, Code[], Code []);

// Code table prototypes
int populateCodeTable(char *, Code[], int);
int codeTableNumCharacters(Code[]);
void initializeCodeTable(Code[], int);
void freeCodeTable(Code[], int);

// Print prototypes
void _printMinHeap(MinHeap *);
void printUsage();

// File prototypes
int writeCharacterCodesToFile(char *, Code[], int);

// MISC prototypes
void freeResources(MinHeap *, MinHeapNode*, Code[], int);


int main(int argc, char **argv)
{
    if(argc != ARG_MIN){
      printUsage();
      return GENERAL_FAILURE;
    }

    /*
    //argv[1] will be "encode" or "decode" signifying the mode of the program
    //for encode mode
        //argv[2] will be the path to input text file
        //argv[3] will be the path to output code table file
        //argv[4] will be the path to output encoded text file
    //for decode mode
        //argv[2] will be the path to input code table file
        //argv[3] will be the path to input encoded text file
        //argv[4] will be the path to output decoded text file
    */
    char *inputTextFilePath = NULL, *codeTableFilePath = NULL, *outputFilePath = NULL;

    if(strcmp(argv[1], ENCODE) == 0){
      /*----------------ENCODER-----------------------*/
      /*----------------------------------------------*/
      
      inputTextFilePath = argv[2];
      codeTableFilePath = argv[3];
      outputFilePath = argv[4];

      // Populate code table
      Code codeTable[CHAR_MAX];
      if(populateCodeTable(inputTextFilePath, codeTable, CHAR_MAX) == ENCODE_FAILURE){
        printf("Could not open file to read: %s\n",inputTextFilePath);
        return ENCODE_FAILURE;
      }
      
      //Count the number of characters that have frequencies
      int numOfCharacters = codeTableNumCharacters(codeTable);

      // Build min heap to be built into huffman tree
      MinHeap * minHeap = NULL;
      if((minHeap = buildMinHeap(codeTable, numOfCharacters)) == NULL){
        printf("Unable to build min heap.\n");
        return ENCODE_FAILURE;
      }

      MinHeapNode * huffmanTree = NULL;

      if((huffmanTree = buildHuffmanTree(minHeap)) == NULL){
        printf("Unable to build huffman tree.\n");
        freeMinHeap(minHeap);
        return ENCODE_FAILURE;
      }

      Code characterCodes[numOfCharacters];
      initializeCodeTable(characterCodes, numOfCharacters);

      if(generateHuffmanCodes(huffmanTree, characterCodes, codeTable) == ENCODE_FAILURE){
        printf("Cannot generate huffman codes.\n");
        freeResources(minHeap, huffmanTree, characterCodes, numOfCharacters);
        return ENCODE_FAILURE;
      }

      // Quicksort before writing to file TODO
      //qSortCodeTable(characterCodes, 0, numOfCharacters-1);

      if(writeCharacterCodesToFile(codeTableFilePath, characterCodes, numOfCharacters-1) == ENCODE_FAILURE){
        printf("Unable to write code table to %s\n", codeTableFilePath);
        freeResources(minHeap, huffmanTree, characterCodes, numOfCharacters);
        return ENCODE_FAILURE;
      }
      // int compressedSize = 0, uncompressed = 0;

      // /*----------------------------------------------*/
      // //To print the statistics about the compression, use print statements as follows
      // printf("Original: %d bits\n", uncompressed*8); //assuming that you store the number of characters in variable "uncompressed". *8 is because ASCII table uses 8 bits to represent each character
      // printf("Compressed: %d bits\n", compressedSize); //assuming that you store the number of bits (i.e., 0/1s) of encoded text in variable "compressed_size"
      // printf("Compression Ratio: %.2f%%\n", (float)compressedSize/((float)uncompressed*8)*100); //This line will print the compression ration in percentages, up to 2 decimals.

      /*----------------------------------------------*/
      //to write encoded version of the text in 0/1 form into text file, you can use a code similar to fprintf statment above that is suggested for writing code table to the file.

      // free resources
      freeResources(minHeap, huffmanTree, characterCodes, numOfCharacters);
      return ENCODE_SUCCESS;
    }else if(strcmp(argv[1], DECODE) == 0){
      /*----------------DECODER-----------------------*/
      /*----------------------------------------------*/
      //When decoding, you will need to:
      //1) read code table: you can use fscanf() function, since the code table file is well structured. Alternatively, you can also use the read statements from above as was used for reading input text file.
      //2) read encoded text, which is a single line consisting of 0/1 characters: This file is better be read character by character, for which you can use a code similar to getc() code above
      //3) write the decoded text into file: for that, you can write it into the file using a code similar to fprintf() usages exemplified above.
      
      printf("UNIMPLEMENTED\n");
      return DECODE_SUCCESS;
    }

    printUsage();
    return GENERAL_FAILURE;
}




void freeResources(MinHeap * minHeap, MinHeapNode* huffmanTree, Code codeTable [], int codeTableSize){
  freeCodeTable(codeTable, codeTableSize);
  freeNode(huffmanTree);
  freeMinHeap(minHeap);
}

int writeCharacterCodesToFile(char * codeTableFilePath, Code characterCodes[], int size){
  FILE *codeTableFile = fopen(codeTableFilePath, "w");
  if (codeTableFile == NULL)
  {
    printf("Could not open file to write: %s\n",codeTableFilePath);
    return ENCODE_FAILURE;
  }

  for(int i = size; i >= 0; i--)
  { 
    if(characterCodes[i].binaryCode != NULL)
      fprintf(codeTableFile, "%c\t%s\t%d\n", characterCodes[i].character, characterCodes[i].binaryCode, characterCodes[i].freq);
  }
  fclose(codeTableFile);
  return ENCODE_SUCCESS;
}

// For debug purposes
void _printMinHeap(MinHeap * minHeap){
  if(minHeap == NULL) return;
  printf("Items in heap before extraction:\n");

  for(int i = 0; i < minHeap->size; i++){
    printf("Character:%c, Frequency:%d\n", minHeap->nodes[i]->character, minHeap->nodes[i]->freq);
  }

  printf("\nExtracting nodes by extract min:\n");
  MinHeapNode * minNode;

  while((minNode = extractMin(minHeap))){
    printf("Character:%c, Frequency:%d\n", minNode->character, minNode->freq);
  }
}

void printUsage(){
  printf("Invalid arugments or not enough arguments supplied.\n");
  printf("Usage: [encode/decode] [path input text file/ path input code table file] [path output code table file/ path input encoded text file] [path output encoded text file/ path output decoded text file]\n");
}

int codeTableNumCharacters(Code codeTable[]){
  int numOfCharacters = 0;
  for(int i = 0; i < CHAR_MAX; i++){
    if(codeTable[i].freq != 0){
      numOfCharacters++;
    }
  }
  return numOfCharacters;
}

int populateCodeTable(char * inputTextFilePath, Code codeTable[], int size){
  FILE *inputFile = fopen(inputTextFilePath, "r");

  if (inputFile == NULL)
    return ENCODE_FAILURE;

  // Initialize frequencies to zero
  for(int i = 0; i < size; i++){
    codeTable[i].freq = 0;
  }


  // Count the frequencies
  int totalNumOfCharacters=0;
  char c;
  while ((c = fgetc(inputFile)) != EOF && c!='\n')
  {
    codeTable[(int)c].freq++;
    totalNumOfCharacters++;
  }
  fclose(inputFile);
  
  return ENCODE_SUCCESS;
}

void initializeCodeTable(Code codeTable[], int size){
  for(int i = 0; i < size; i++){
    codeTable[i].binaryCode = NULL;
    codeTable[i].freq = 0;
  }
}

void freeCodeTable(Code codeTable[], int size){
  for(int i = 0; i < size; i++){
      if(codeTable[i].binaryCode != NULL){
        free(codeTable[i].binaryCode);
      }
  }
}

int generateHuffmanCodes(MinHeapNode* root, Code characterCodes[], Code codeTable[]) {
  if (root == NULL) {
      return ENCODE_FAILURE;
  }

  char code[CHAR_MAX];
  int index = 0;
  generateHuffmanCodesH(root, code, 0, &index, characterCodes, codeTable);
  return ENCODE_SUCCESS;
}

void generateHuffmanCodesH(MinHeapNode* node, char * code, int level, int *i, Code characterCodes[], Code codeTable[]) {
  if (node == NULL) return;

  if (node->character != INTERNAL_NODE_CHARACTER) {
      code[level] = '\0';
      characterCodes[*i].character = node->character;
      characterCodes[*i].freq = node->freq;
      characterCodes[*i].binaryCode = strdup(code);
      codeTable[(int)node->character].binaryCode = characterCodes[*i].binaryCode;

      // Debug output
      printf("Character: %c, Code: %s, Frequency: %d\n", node->character, code, node->freq);

      (*i)++;
  }

  code[level] = '0';
  generateHuffmanCodesH(node->left, code, level + 1, i, characterCodes, codeTable);
  code[level] = '1';
  generateHuffmanCodesH(node->right, code, level + 1, i, characterCodes, codeTable);
}


MinHeapNode* buildHuffmanTree(MinHeap * minHeap){
  if(minHeap == NULL || minHeap->size <= 0){
    printf("Empty minheap, cannot build huffman tree.\n");
    return NULL;
  }

  MinHeapNode * leftNode, *rightNode, *newNode;

  while (minHeap->size > 1) {
    leftNode = extractMin(minHeap);
    rightNode = extractMin(minHeap);

    if (leftNode == NULL || rightNode == NULL) {
      printf("Error: Failed to extract nodes from the heap.\n");
      return NULL;
    }

    newNode = createNode(INTERNAL_NODE_CHARACTER, leftNode->freq + rightNode->freq);
    if (newNode == NULL) return NULL;

    if(leftNode->character < rightNode->character){
      
    }
    newNode->left = leftNode;
    newNode->right = rightNode;

    insertMinHeap(minHeap, newNode);
  }

  return extractMin(minHeap);
}

MinHeap * createMinHeap(int capacity){
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    
    if(minHeap == NULL){
      perror("Unable to allocate memory for min heap.\n");
      return NULL;
    }

    minHeap->size = 0; 
    minHeap->capacity = capacity; 

    minHeap->nodes = (MinHeapNode**)calloc(capacity, sizeof(MinHeapNode*)); 

    if(minHeap->nodes == NULL){
      perror("Unable to allocate memory for nodes.\n");
      return NULL;
    }

    return minHeap; 
}

void swapMinHeap(MinHeapNode ** i, MinHeapNode ** j){
  MinHeapNode * temp = *i;
  *i = *j;
  *j = temp;
}

MinHeapNode* createNode(char character, int freq){ 
    MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode)); 

    if(temp == NULL){
      perror("Unabl to create node for heap.\n");
      return NULL;
    }

    temp->left = NULL;
    temp->right = NULL;
    temp->character = character; 
    temp->freq = freq;
    return temp; 
} 

int minHeapLChild(int i){
  return 2 * i + 1;
}

int minHeapRChild(int i){
  return 2 * i + 2;
}

int minHeapParent(int i){
  return (i - 1)/2;
}

void downHeap(MinHeap* minHeap, int i){ 
  
    int smallest = i; 
    int left = minHeapLChild(i); 
    int right = minHeapRChild(i); 
  
    if (left < minHeap->size && minHeap->nodes[left]->freq < minHeap->nodes[smallest]->freq) 
        smallest = left; 
  
    if (right < minHeap->size && minHeap->nodes[right]->freq < minHeap->nodes[smallest]->freq) 
        smallest = right; 

    if (smallest != i) { 
        swapMinHeap(&minHeap->nodes[smallest], &minHeap->nodes[i]); 
        downHeap(minHeap, smallest); 
    } 
} 

MinHeapNode* extractMin(MinHeap* minHeap){
  if(minHeap == NULL || minHeap->size == 0) return NULL;
  MinHeapNode* minNode = minHeap->nodes[0]; 
  //minHeap->nodes[0] = minHeap->nodes[minHeap->size - 1]; 
  swapMinHeap(&minHeap->nodes[0], &minHeap->nodes[minHeap->size - 1]);

  --minHeap->size; 
  downHeap(minHeap, 0); 

  return minNode; 
} 

void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode){ 
  ++minHeap->size; 
  int i = minHeap->size - 1; 

  while (i && minHeapNode->freq < minHeap->nodes[minHeapParent(i)]->freq) { 
      swapMinHeap(&minHeap->nodes[i], &minHeap->nodes[minHeapParent(i)]); 
      i = minHeapParent(i); 
  } 

  swapMinHeap(&minHeap->nodes[i], &minHeapNode);  
} 

void minHeapify(MinHeap* minHeap){
    if(minHeap == NULL){
      fprintf(stderr, "Cannot min heapify a null heap.\n");
      return;
    } 
  
    int n = minHeap->size - 1; 
    int i; 
  
    for (i = (n - 1) / 2; i >= 0; --i) 
        downHeap(minHeap, i); 
} 

void freeMinHeap(MinHeap *minHeap) {
  if (minHeap == NULL) {
    return;
  }

  free(minHeap->nodes);
  free(minHeap);
}

void freeNode(MinHeapNode *node) {
  if (node == NULL) {
    return;
  }

  freeNode(node->left);
  freeNode(node->right);

  free(node);
}

MinHeap * buildMinHeap(Code codeTable[], int numOfCharacters){
  MinHeap * minHeap;

  if((minHeap = createMinHeap(numOfCharacters)) == NULL){
    return NULL;
  }

  int minHeapSize = 0;
  for (int i = 0; i < CHAR_MAX; i++)
  {
    if(minHeapSize == numOfCharacters) break;
    if(codeTable[i].freq != 0){
      if((minHeap->nodes[minHeapSize++] = createNode(i, codeTable[i].freq)) == NULL){
        printf("Unable to create node for min heap.\n");
        freeMinHeap(minHeap);
        return NULL;
      }
    }
  }

  minHeap->size = minHeapSize;
  minHeapify(minHeap);
  return minHeap;
}
