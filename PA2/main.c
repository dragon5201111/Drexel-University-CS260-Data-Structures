#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define BUFSIZE 256

typedef struct item{
  char *word;
  int weight;
}Item;


int compare_items_by_word(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    return strcmp(item1->word, item2->word); // Compare words alphabetically
}


void freeDict(Item *, int);

void swap(Item *, int, int);
int compareItem(Item *, int, int, int);
void qSortH(Item *, int, int, int);
void qSort(Item *, int, int);
int partition(Item *, int, int, int);

int binSearchItemsH(Item *, char *, int, int, int);
int binSearchItems(Item *, char *, int);

void procQueries(Item *, int, char **, int);
void printSuggestions(Item *, int, char *, int);


void freeDict(Item *dict, int size) {
    for (int i = 0; i < size; i++) {
        free(dict[i].word);
    }
    free(dict);
}

void swap(Item *item, int i, int j){
    Item temp = item[i];
    item[i] = item[j];
    item[j] = temp;
}

int compareItem(Item *item, int i, int j, int sortByWord) {
    if (sortByWord) return strcmp(item[i].word, item[j].word);

    return item[j].weight - item[i].weight;
}

int partition(Item *items, int low, int high, int sortByWord) {
    int randIndx = low + rand() % (high - low + 1);

    swap(items, randIndx, high);

    Item pivot = items[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compareItem(items, j, high, sortByWord) < 0) {
            i++;
            swap(items, i, j);
        }
    }

    swap(items, i + 1, high);
    return i + 1;
}

void qSortH(Item *items, int low, int high, int sortByWord) {
    if (low < high) {
        int piv = partition(items, low, high, sortByWord);
        qSortH(items, low, piv - 1, sortByWord);
        qSortH(items, piv + 1, high, sortByWord);
    }
}

void qSort(Item *items, int size, int sortByWord) {
    qSortH(items, 0, size - 1, sortByWord);
}

int binSearchItemsH(Item *items, char *target, int targetLen, int low, int high) {
    int result = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        char *currentWord = items[mid].word;
        
        int cmpResult = strncmp(target, currentWord, targetLen);

        if (cmpResult == 0) {
            result = mid;
            high = mid - 1;
        } else if (cmpResult < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return result;
}

int binSearchItems(Item *items, char *target, int high) {
    return binSearchItemsH(items, target, strlen(target), 0, high - 1);
}

void procQueries(Item *dict, int dictSize, char **queries, int queryCount) {
    for (int i = 0; i < queryCount; i++) {
        printSuggestions(dict, dictSize, queries[i], strlen(queries[i]));
    }
}

void printSuggestions(Item *dict, int dictSize, char *query, int queryLen) {
    printf("Query word:%s\n", query);
    int low = binSearchItems(dict, query, dictSize);
    if (low == -1) {
        printf("No suggestion!\n");
        return;
    }

    // Cannot assume matches could not be all of the dictionary
    Item * matchBuffer = malloc(sizeof(Item) * dictSize);
    int matchCount = 0;
    int i = low;
    
    // Fill buffer while there is a match and match count less than dictionary size
    for (; i < dictSize && strncmp(query, dict[i].word, queryLen) == 0 && matchCount < dictSize; i++) {
        matchBuffer[matchCount++] = dict[i];
    }

    // Sort buffer numerically (i.e., by weight)
    qSort(matchBuffer, matchCount, 0);

    // Print top 10, or at least try to
    for(int j = 0; j < matchCount && j < 10; j++){
        printf("%s %d\n", matchBuffer[j].word, matchBuffer[j].weight);
    }

    free(matchBuffer);

}

int main(int argc, char **argv) {
    srand(time(NULL)); // For quicksort function (i.e., random pivot selection)

    char *dictionaryFilePath = argv[1]; //this keeps the path to dictionary file
    char *queryFilePath = argv[2]; //this keeps the path to the file that keeps a list of query wrods, 1 query per line
    int wordCount=0; //this variable will keep a count of words in the dictionary, telling us how much memory to allocate
    int queryCount=0; //this variable will keep a count of queries in the query file, telling us how much memory to allocate for the query words
    
    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read dictionary file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    FILE *fp = fopen(dictionaryFilePath, "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    ssize_t lineSize; //variable to be used for line counting
    
    //check if the file is accessible, just to make sure...
    if(fp == NULL){
        fprintf(stderr, "Error opening file:%s\n",dictionaryFilePath);
        return -1;
    }

    //First, let's count number of lines. This will help us know how much memory to allocate
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
    {
        wordCount++;
    }

    //Printing wordCount for debugging purposes. You can remove this part from your submission.
    //printf("%d\n",wordCount);
    
    /////////////////PAY ATTENTION HERE/////////////////
    //This might be a good place to allocate memory for your data structure, by the size of "wordCount"
    ////////////////////////////////////////////////////

    Item * dictWords = (Item *) malloc(sizeof(Item) * wordCount);
    
    if(dictWords == NULL){
        fprintf(stderr, "Unable to allocate space for dictionary words.\n");
        return -1;
    }
    
    //Read the file once more, this time to fill in the data into memory
    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    char word[BUFSIZE]; //to be used for reading lines in the loop below
    int weight;

    for(int i = 0; i < wordCount; i++)
    {
        fscanf(fp, "%s %d\n",word,&weight);
        //Let's print them to the screen to make sure we can read input, for debugging purposes. You can remove this part from your submission.
        //printf("%s %d\n",word,weight);

        /////////////////PAY ATTENTION HERE/////////////////
        //This might be a good place to store the dictionary words into your data structure
        ////////////////////////////////////////////////////
        dictWords[i].word = strdup(word);  // Copy word into the structure
        dictWords[i].weight = weight; 
    }


    // Sort dictionary alphabetically
    qSort(dictWords, wordCount, 1);

    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read query list file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    fp = fopen(queryFilePath, "r");
        
    //check if the file is accessible, just to make sure...
    if(fp == NULL){
        freeDict(dictWords, wordCount);
        fprintf(stderr, "Error opening file:%s\n",queryFilePath);
        return -1;
    }

    //First, let's count number of queries. This will help us know how much memory to allocate
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
    {
        queryCount++;
    }
    free(line); //getline internally allocates memory, so we need to free it here so as not to leak memory!!

    //Printing line count for debugging purposes. You can remove this part from your submission.
    //printf("%d\n",queryCount);

    /////////////////PAY ATTENTION HERE/////////////////
    //This might be a good place to allocate memory for storing query words, by the size of "queryCount"
    ////////////////////////////////////////////////////
    char ** queryWords = (char **)malloc(sizeof(char *) * queryCount);

    if(!queryWords){
        fprintf(stderr, "Unable to allocate memory for query words.\n");
        return -1;
    }

    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    for(int i = 0; i < queryCount; i++)
    {
        fscanf(fp, "%s\n",word);
        //Let's print them to the screen to make sure we can read input, for debugging purposes. You can remove this part from your submission.
        //printf("%s %d\n",word);
        
        /////////////////PAY ATTENTION HERE/////////////////
        //This might be a good place to store the query words in a list like data structure
        ////////////////////////////////////////////////////
        queryWords[i] = strdup(word);
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// reading input is done ////////////////////////
    ////////////////////////////////////////////////////////////////////////
    
    //Now it is your turn to do the magic!!!
    //do search/sort/print, whatever you think you need to do to satisfy the requirements of the assignment!
    //loop through the query words and list suggestions for each query word if there are any
    //don't forget to free the memory before you quit the program!
    
    //OUTPUT SPECS:
    // use the following if no word to suggest: printf("No suggestion!\n");
    // use the following to print a single line of outputs (assuming that the word and weight are stored in variables named word and weight, respectively): 
    // printf("%s %d\n",word,weight);
    // if there are more than 10 outputs to print, you should print the top 10 weighted outputs.

    //Process and print queries (i.e., the bulk of this program)
    procQueries(dictWords, wordCount, queryWords, queryCount);

    // Free dictionary and query words
    freeDict(dictWords, wordCount);

    for(int i = 0; i < queryCount; i++){
        free(queryWords[i]);
    }

    free(queryWords);
    return 0;
}
