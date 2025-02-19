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

// a struct to keep frequency and binary code representation of a character
typedef struct code{
  unsigned int freq;
  char character;
  char *binaryCode;
} Code;


typedef struct bstNode{
  Code code;
  struct bstNode * left;
  struct bstNode * right;
}BSTNode;

// A priority queue (min heap) to hold codes
typedef struct nodeHeap{
  BSTNode * nodes;
  int size;
  int capacity;
} NodeHeap;

/*----------------Heap Functions-----------------------*/
void swapNodeHeap(NodeHeap *nodeHeap, int i, int j) {
  BSTNode temp = nodeHeap->nodes[i];
  nodeHeap->nodes[i] = nodeHeap->nodes[j];
  nodeHeap->nodes[j] = temp;
}

void initNodeHeap(NodeHeap * nodeHeap, int size){
  if(size < 0){
    printf("Size %d out of bounds. Cannot initialize heap.\n", size);
    return;
  }

  if(nodeHeap == NULL){
    printf("Cannot initialize null heap.\n");
    return;
  }

  if((nodeHeap->nodes = (BSTNode *) calloc(size, sizeof(BSTNode))) == NULL){
    perror("Unable to allocate memory for heap.\n");
    return;
  }

  nodeHeap->capacity = size;
  nodeHeap->size = 0;
}

void freeNodes(BSTNode * bstNode){
  if(bstNode == NULL){
    return;
  }

  freeNodes(bstNode->left);
  freeNodes(bstNode->right);

  if (bstNode->code.binaryCode != NULL) {
    free(bstNode->code.binaryCode);
  }

  free(bstNode);
}

void freeNodeHeap(NodeHeap * nodeHeap){
  if(nodeHeap == NULL){
    printf("Cannot free null heap.\n");
    return;
  }

  if(nodeHeap->nodes == NULL){
    printf("Cannot free null nodes.\n");
    return;
  }

  for (int i = 0; i < nodeHeap->capacity; i++)
  {
    if(nodeHeap->nodes[i].code.binaryCode != NULL){
      free(nodeHeap->nodes[i].code.binaryCode);
    }
    freeNodes(nodeHeap->nodes[i].left);
    freeNodes(nodeHeap->nodes[i].right);

  }
  
  free(nodeHeap->nodes);
  nodeHeap->size = 0;
}

int getLChildNodeHeap(int i){
  return (2*i)+1;
}

int getRChildNodeHeap(int i){
  return (2*i)+2;
}

int getParentNodeHeap(int i){
  return (i - 1) / 2;
}

void upNodeHeap(NodeHeap *nodeHeap, int i) {
  int parentIndex = getParentNodeHeap(i);

  if (parentIndex < 0) return;

  if (nodeHeap->nodes[i].code.freq < nodeHeap->nodes[parentIndex].code.freq) {
    swapNodeHeap(nodeHeap, i, parentIndex);
    upNodeHeap(nodeHeap, parentIndex);
  }
}

void insertNodeHeap(NodeHeap *nodeHeap, char character, int freq) {
  if (nodeHeap == NULL) {
    printf("Cannot insert into null heap.\n");
    return;
  }

  if (nodeHeap->size >= nodeHeap->capacity) {
    printf("Heap is full! Cannot insert more nodes.\n");
    return;
  }

  nodeHeap->nodes[nodeHeap->size].code.character = character;
  nodeHeap->nodes[nodeHeap->size].code.freq = freq;
  nodeHeap->size++;

  upNodeHeap(nodeHeap, nodeHeap->size - 1);
}

void downNodeHeap(NodeHeap *nodeHeap, int i) {
  int leftChild = getLChildNodeHeap(i);
  int rightChild = getRChildNodeHeap(i);
  int smallest = i;

  if (leftChild < nodeHeap->size && nodeHeap->nodes[leftChild].code.freq < nodeHeap->nodes[smallest].code.freq) {
    smallest = leftChild;
  }

  if (rightChild < nodeHeap->size && nodeHeap->nodes[rightChild].code.freq < nodeHeap->nodes[smallest].code.freq) {
    smallest = rightChild;
  }

  if (smallest != i) {
    swapNodeHeap(nodeHeap, i, smallest);
    downNodeHeap(nodeHeap, smallest);
  }
}

BSTNode* extractMinNodeHeap(NodeHeap *nodeHeap) {
  if (nodeHeap == NULL || nodeHeap->size == 0) {
    return NULL;
  }


  swapNodeHeap(nodeHeap, 0, nodeHeap->size - 1);
  BSTNode * minNode = &nodeHeap->nodes[nodeHeap->size - 1];

  nodeHeap->size--;
  downNodeHeap(nodeHeap, 0);
  return minNode;
}


void printUsage(){
  printf("Invalid arugments or not enough arguments supplied.\n");
  printf("Usage: [encode/decode] [path input text file/ path input code table file] [path output code table file/ path input encoded text file] [path output encoded text file/ path output decoded text file]\n");
}

int main(int argc, char **argv)
{
    if(argc != 5){
      printUsage();
      return GENERAL_FAILURE;
    }

    //argv[1] will be "encode" or "decode" signifying the mode of the program
    //for encode mode
        //argv[2] will be the path to input text file
        //argv[3] will be the path to output code table file
        //argv[4] will be the path to output encoded text file
    //for decode mode
        //argv[2] will be the path to input code table file
        //argv[3] will be the path to input encoded text file
        //argv[4] will be the path to output decoded text file
    char *inputTextFilePath = NULL, *codeTableFilePath = NULL, *outputFilePath = NULL;

    if(strcmp(argv[1], ENCODE) == 0){
      /*----------------ENCODER-----------------------*/
      /*----------------------------------------------*/
      
      inputTextFilePath = argv[2];
      codeTableFilePath = argv[3];
      outputFilePath = argv[4];

      /*To read the input text file, you might want to use a code as follows*/
      FILE *inputFile = fopen(inputTextFilePath, "r");

      if (inputFile == NULL)
      {
        printf("Could not open file to read: %s\n",inputTextFilePath);
        return ENCODE_FAILURE;
      }

      Code codeTable[CHAR_MAX];
      //set counters to zero initially
      for(int i = 0; i < CHAR_MAX; i++){
        codeTable[i].freq = 0;
        codeTable[i].character = i;
      }

      int totalNumOfCharacters=0;
      char c;
      //read the file character by character
      while ((c = fgetc(inputFile)) != EOF && c!='\n')
      {
        //increase the frequency of the character by one
        codeTable[(int)c].freq++;
        //increase the total character count
        totalNumOfCharacters++;
      }
      fclose(inputFile);

      //now that you have collected the frequency of each character that is present 
      //in the input file, you need to generate the code table.
      int numOfCharacters = 0;
      for(int i = 0; i < CHAR_MAX; i++){
        if(codeTable[i].freq != 0) numOfCharacters++;
      }

      // Hint: when initializing the heap to be used in the generation of Huffman
      // 		 tree, keep in mind that you know how many unique characters have
      //		 appeared in the input text. You can get this number by going through
      //		 your codeTable...
      NodeHeap nodeHeap = {0};
      initNodeHeap(&nodeHeap, numOfCharacters);

      // Insert into heap
      for (int i = 0; i < CHAR_MAX; i++)
      {
        if(codeTable[i].freq != 0){
          //printf("Inserting:%c, Freq:%d\n", codeTable[i].character, codeTable[i].freq);
          insertNodeHeap(&nodeHeap, codeTable[i].character, codeTable[i].freq);
        }
      }

      
      // printf("\nIn code heap:\n");
      // for (int i = 0; i < nodeHeap.capacity; i++)
      // {
      //   printf("Code: %c, Freq:%d\n", nodeHeap.nodes[i].code.character, nodeHeap.nodes[i].code.freq);
      // }
      
      // BSTNode * minNode = extractMinNodeHeap(&nodeHeap);

      // printf("\nExtracting Min(s):\n");
      // while(minNode != NULL){
      //   printf("Extracted: %c, %d\n", minNode->code.character, minNode->code.freq);
      //   minNode = extractMinNodeHeap(&nodeHeap);
      // }      

      // putchar('\n');

      /*----------------------------------------------*/
      //to write the code table into the file, you might want to use a code as follows
      FILE *codeTableFile = fopen(codeTableFilePath, "w");
      if (codeTableFile == NULL)
      {
        printf("Could not open file to write: %s\n",codeTableFilePath);
        return ENCODE_FAILURE;
      }

      // Write the code table into file:
      for(int i = numOfCharacters - 1; i >= 0; i--)
      {
        // IMPLEMENT THIS LATER
        //fprintf(codeTableFile, "%c\t%s\t%d\n", characters[i], characterCodes[i], characterFrequencies[i]);
      }
      fclose(codeTableFile);


      int compressedSize = 0, uncompressed = 0;

      /*----------------------------------------------*/
      //To print the statistics about the compression, use print statements as follows
      printf("Original: %d bits\n", uncompressed*8); //assuming that you store the number of characters in variable "uncompressed". *8 is because ASCII table uses 8 bits to represent each character
      printf("Compressed: %d bits\n", compressedSize); //assuming that you store the number of bits (i.e., 0/1s) of encoded text in variable "compressed_size"
      printf("Compression Ratio: %.2f%%\n", (float)compressedSize/((float)uncompressed*8)*100); //This line will print the compression ration in percentages, up to 2 decimals.

      /*----------------------------------------------*/
      //to write encoded version of the text in 0/1 form into text file, you can use a code similar to fprintf statment above that is suggested for writing code table to the file.
      freeNodeHeap(&nodeHeap);
      return ENCODE_SUCCESS;
    }else if(strcmp(argv[1], DECODE) == 0){
      /*----------------DECODER-----------------------*/
      /*----------------------------------------------*/
      //When decoding, you will need to:
      //1) read code table: you can use fscanf() function, since the code table file is well structured. Alternatively, you can also use the read statements from above as was used for reading input text file.
      //2) read encoded text, which is a single line consisting of 0/1 characters: This file is better be read character by character, for which you can use a code similar to getc() code above
      //3) write the decoded text into file: for that, you can write it into the file using a code similar to fprintf() usages exemplified above.
    
    
      return DECODE_SUCCESS;
    }

    printUsage();
    return GENERAL_FAILURE;
}
