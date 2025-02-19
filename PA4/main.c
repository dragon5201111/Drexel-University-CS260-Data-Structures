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

// a struct to keep frequency and binary code representation of a character
typedef struct code{
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
void downHeap(MinHeap*, int);
int minHeapLChild(int);
int minHeapRChild(int);
int minHeapParent(int);
void insertMinHeap(MinHeap*,MinHeapNode*); 
void minHeapify(MinHeap*);
void freeMinHeap(MinHeap *);
void freeNode(MinHeapNode *);
void swapMinHeap(MinHeapNode **, MinHeapNode **);

// Code table prototypes
int populateCodeTable(char *, Code[]);
int codeTableNumCharacters(Code[]);

// Print prototypes
void _printMinHeap(MinHeap *);
void printUsage();

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
      if(populateCodeTable(inputTextFilePath, codeTable) == ENCODE_FAILURE){
        printf("Could not open file to read: %s\n",inputTextFilePath);
        return ENCODE_FAILURE;
      }
      
      //Count the number of characters that have frequencies
      int numOfCharacters = codeTableNumCharacters(codeTable);

      // Build min heap to be built into huffman tree
      MinHeap * minHeap = buildMinHeap(codeTable, numOfCharacters);

      // FILE *codeTableFile = fopen(codeTableFilePath, "w");
      // if (codeTableFile == NULL)
      // {
      //   printf("Could not open file to write: %s\n",codeTableFilePath);
      //   return ENCODE_FAILURE;
      // }

      // // Write the code table into file:
      // for(int i = numOfCharacters - 1; i >= 0; i--)
      // {
      //   // IMPLEMENT THIS LATER
      //   //fprintf(codeTableFile, "%c\t%s\t%d\n", characters[i], characterCodes[i], characterFrequencies[i]);
      // }
      // fclose(codeTableFile);


      // int compressedSize = 0, uncompressed = 0;

      // /*----------------------------------------------*/
      // //To print the statistics about the compression, use print statements as follows
      // printf("Original: %d bits\n", uncompressed*8); //assuming that you store the number of characters in variable "uncompressed". *8 is because ASCII table uses 8 bits to represent each character
      // printf("Compressed: %d bits\n", compressedSize); //assuming that you store the number of bits (i.e., 0/1s) of encoded text in variable "compressed_size"
      // printf("Compression Ratio: %.2f%%\n", (float)compressedSize/((float)uncompressed*8)*100); //This line will print the compression ration in percentages, up to 2 decimals.

      /*----------------------------------------------*/
      //to write encoded version of the text in 0/1 form into text file, you can use a code similar to fprintf statment above that is suggested for writing code table to the file.
      
      // free resources
      freeMinHeap(minHeap);
      return ENCODE_SUCCESS;
    }else if(strcmp(argv[1], DECODE) == 0){
      /*----------------DECODER-----------------------*/
      /*----------------------------------------------*/
      //When decoding, you will need to:
      //1) read code table: you can use fscanf() function, since the code table file is well structured. Alternatively, you can also use the read statements from above as was used for reading input text file.
      //2) read encoded text, which is a single line consisting of 0/1 characters: This file is better be read character by character, for which you can use a code similar to getc() code above
      //3) write the decoded text into file: for that, you can write it into the file using a code similar to fprintf() usages exemplified above.
      
      printf("IMPLEMENT\n");

      inputTextFilePath = argv[3];
      codeTableFilePath = argv[2];
      outputFilePath = argv[4];

      return DECODE_SUCCESS;
    }

    printUsage();
    return GENERAL_FAILURE;
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

int populateCodeTable(char * inputTextFilePath, Code codeTable[]){
  FILE *inputFile = fopen(inputTextFilePath, "r");

  if (inputFile == NULL)
    return ENCODE_FAILURE;

  // Initialize frequencies to zero
  for(int i = 0; i < CHAR_MAX; i++){
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

    temp->left = temp->right = NULL; 
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
    if(minHeap == NULL || minHeapNode == NULL){
      fprintf(stderr, "Cannot insert to null heap or null node.\n");
      return;
    }
  
    ++minHeap->size; 
    int i = minHeap->size - 1; 
    
    while (i && minHeapNode->freq < minHeap->nodes[minHeapParent(i)]->freq) { 
        minHeap->nodes[i] = minHeap->nodes[minHeapParent(i)]; 
        i = minHeapParent(i); 
    } 
  
    minHeap->nodes[i] = minHeapNode; 
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

void freeNode(MinHeapNode * minHeapNode){
  if(minHeapNode == NULL) return;

  freeNode(minHeapNode->left);
  freeNode(minHeapNode->right);

  free(minHeapNode);
}

void freeMinHeap(MinHeap * minHeap){
  if(minHeap == NULL){
    return;
  }

  for(int i = 0; i < minHeap->capacity; i++){
    freeNode(minHeap->nodes[i]);
  }

  free(minHeap->nodes);
  free(minHeap);
}

MinHeap * buildMinHeap(Code codeTable[], int numOfCharacters){
  MinHeap * minHeap = createMinHeap(numOfCharacters);

  int minHeapSize = 0;
  for (int i = 0; i < CHAR_MAX; i++)
  {
    if(minHeapSize == numOfCharacters) break;
    if(codeTable[i].freq != 0){
      if((minHeap->nodes[minHeapSize++] = createNode(i, codeTable[i].freq)) == NULL){
        printf("Unable to create node for min heap.\n");
        freeMinHeap(minHeap);
      }
    }
  }

  minHeap->size = minHeapSize;
  minHeapify(minHeap);
  return minHeap;
}