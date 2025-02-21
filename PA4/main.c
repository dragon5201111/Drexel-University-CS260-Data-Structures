#include <stdio.h>
#include <string.h>


//==========Macros==========
// Return Codes
#define INVALID_ARGS -1
#define GENERAL_FAILURE -2
#define ENCODE_SUCCESS 0
#define DECODE_SUCCESS 1
#define FILE_ERROR -2
// For Arguments
#define ARG_MAX 5
#define ENCODE "encode"
#define DECODE "decode"
// MISC
#define CHAR_MAX 255

//==========Function Prototypes==========
void printUsage();
int countFrequencies(char *, int[], int);



int main(int argc, char ** argv){
    char *inputTextFilePath = NULL, *codeTableFilePath = NULL, *outputFilePath = NULL;

    if(argc != ARG_MAX){
        printUsage();
        return INVALID_ARGS;
    }
    
    if(strcmp(argv[1], ENCODE) == 0){
        inputTextFilePath = argv[2];
        codeTableFilePath = argv[3];
        outputFilePath = argv[4];

        int numOfCharacters, frequencies[CHAR_MAX];
        if((numOfCharacters = countFrequencies(inputTextFilePath, frequencies, CHAR_MAX)) == FILE_ERROR){
            printf("Unable to read file: %s for frequencies.\n", inputTextFilePath);
            return FILE_ERROR;
        }

        
        return ENCODE_SUCCESS;
    }else if(strcmp(argv[1], DECODE) == 0){


        return DECODE_SUCCESS;
    }

    // Invalid argv[1]
    return GENERAL_FAILURE;
}

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
        frequencies[i] = 0;
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