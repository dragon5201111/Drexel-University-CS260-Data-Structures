#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOAD_FACTOR 0.75
#define BUFSIZE 256

typedef struct Node{
    struct Node * next;
    char * key;
}Node;

typedef struct LinkedList{
    struct Node * head;
}LinkedList;

typedef struct HashTable{
    LinkedList ** buckets;
    int capacity;
    int size;
}HashTable;

void _initializeBuckets(LinkedList ** buckets, int capacity){
    for (int i = 0; i < capacity; i++) {
        buckets[i] = (LinkedList *)malloc(sizeof(LinkedList));
        if (buckets[i] == NULL) {
            printf("Unable to initialize bucket %d.\n", i);
            return;
        }
        buckets[i]->head = NULL; // Make sure the bucket is initialized properly
    }
}

float _getLoadFactor(HashTable * hashTable){
    return (float)hashTable->size / hashTable->capacity;
}

int _getHash(char *s) {
    //DJB2 hash function
    unsigned long hash = 5381;
    char c;
    while((c = *s++)){
        hash = ((hash << 5) + hash) + c;
    }

    // Change msb to a 0 in case of overflow
    return (hash & 0x7fffffff);
}

int getHashKey(char * s, int capacity){
    return _getHash(s) % capacity;
}

void initializeHashTable(HashTable *hashTable, int capacity) {
    if (hashTable == NULL) {
        printf("Cannot initialize null hashtable.\n");
        return;
    }

    hashTable->capacity = capacity;
    hashTable->size = 0;

    hashTable->buckets = (LinkedList **)malloc(sizeof(LinkedList *) * capacity);
    if (hashTable->buckets == NULL) {
        printf("Unable to initialize hashtable buckets.\n");
        return;
    }

    _initializeBuckets(hashTable->buckets, capacity);
}

void freeHashTable(HashTable *hashTable) {
    if (hashTable == NULL) {
        printf("Cannot free null hashtable.\n");
        return;
    }

    for (int i = 0; i < hashTable->capacity; i++) {
        LinkedList *currentBucket = hashTable->buckets[i];
        Node *currentNode = currentBucket->head;
        Node * temp = NULL;

        while (currentNode != NULL) {
            temp = currentNode;
            currentNode = currentNode->next;
            free(temp->key);
            free(temp);
        }
        free(currentBucket);
    }

    free(hashTable->buckets);
}

Node * _createNode(char *key) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) {
        printf("Unable to initialize node for key: %s\n", key);
        return NULL;
    }

    int keyLen = strlen(key);
    node->key = (char *) malloc((keyLen + 1) * sizeof(char));
    if (node->key == NULL) {
        printf("Unable to initialize key for node: %s\n", key);
        free(node);
        return NULL;
    }

    strcpy(node->key, key);

    node->next = NULL;

    return node;
}

void _resizeHashTable(HashTable *hashTable) {
    int newCapacity = hashTable->capacity * 2;
    LinkedList **newBuckets = (LinkedList **)malloc(sizeof(LinkedList *) * newCapacity);

    if (newBuckets == NULL) {
        printf("Unable to create new buckets for resizing.\n");
        return;
    }

    _initializeBuckets(newBuckets, newCapacity);

    Node *tempNode, *currentNode;

    // Rehash buckets
    for (int i = 0; i < hashTable->capacity; i++) {
        LinkedList *currentBucket = hashTable->buckets[i];
        currentNode = currentBucket->head;

        while (currentNode != NULL) {
            int hashKey = getHashKey(currentNode->key, newCapacity);

            Node *nodeCopy = _createNode(currentNode->key);
            nodeCopy->next = newBuckets[hashKey]->head;
            newBuckets[hashKey]->head = nodeCopy;

            tempNode = currentNode;
            currentNode = currentNode->next;
            free(tempNode->key);
            free(tempNode);
        }

        free(currentBucket);
    }

    free(hashTable->buckets);
    hashTable->capacity = newCapacity;
    hashTable->buckets = newBuckets;
}

void insertHashTable(HashTable * hashTable, char * key){
    if(hashTable == NULL || key == NULL ){
        printf("Key or hashtable is null. Unable to insert\n");
        return;
    }
    
    if(_getLoadFactor(hashTable) > DEFAULT_LOAD_FACTOR){
        _resizeHashTable(hashTable);
    }

    Node * node = _createNode(key);

    if(node == NULL){
        printf("Unable to create node. Insertion failed.\n");
        return;
    }

    LinkedList * bucket = hashTable->buckets[getHashKey(key, hashTable->capacity)];

    node->next = bucket->head;
    bucket->head = node;
    hashTable->size++;
}

Node * searchHashTable(HashTable * hashTable, char * key){
    if(hashTable == NULL){
        printf("Cannot search null hashtable.\n");
        return NULL;
    }

    int hashKey = getHashKey(key, hashTable->capacity);

    LinkedList * bucket = hashTable->buckets[hashKey];
    Node * head = bucket->head;

    while(head != NULL){
        if(strcmp(head->key, key) == 0){
            return head;
        }
        head = head->next;
    }

    return NULL;
}

int main(int argc, char **argv)
{
	char *dictionaryFilePath = argv[1]; //this keeps the path to the dictionary file file
	char *inputFilePath = argv[2]; //this keeps the path to the input text file
	char *check = argv[3]; // this keeps the flag to whether we should insert mistyped words into dictionary or ignore
	int numOfWords=0; //this variable will tell us how much memory to allocate

	int insertToDictionary = strcmp(check,"add")==0 ? 1 : 0;
    
	////////////////////////////////////////////////////////////////////
	//read dictionary file
    FILE *fp = fopen(dictionaryFilePath, "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    ssize_t lineSize; //variable to be used for line counting

    //check if the file is accessible, just to make sure...
    if(fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    //First, let's count number of words in the dictionary.
    //This will help us know how much memory to allocate for our hash table
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
        numOfWords++;

    //Printing line count for debugging purposes.
    //You can remove this part from your submission.
    //printf("%d\n",numOfWords);
    
    //HINT: You can initialize your hash table here, since you know the size of the dictionary
    HashTable hashTable;
    initializeHashTable(&hashTable, numOfWords);

    //rewind file pointer to the beginning of the file, to be able to read it line by line.
    fseek(fp, 0, SEEK_SET);

    char wrd[BUFSIZE];
    for (int i = 0; i < numOfWords; i++)
    {
        fscanf(fp, "%s \n", wrd);
        //You can print the words for Debug purposes, just to make sure you are loading the dictionary as intended
        insertHashTable(&hashTable, wrd);        
        //HINT: here is a good place to insert the words into your hash table
    }
    fclose(fp);
    
	////////////////////////////////////////////////////////////////////
	//read the input text file word by word
    fp = fopen(inputFilePath, "r");
	
	//check if the file is accessible, just to make sure...
	if(fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

    //HINT: You can use a flag to indicate if there is a misspleed word or not, which is initially set to 1
	int noTypo=1;

	//read a line from the input file
	while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
	{
		char *word;
        //These are the delimiters you are expected to check for. Nothing else is needed here.
		const char delimiter[]= " ,.:;!\n";

		//split the buffer by delimiters to read a single word
		word = strtok(line,delimiter); 
		
		//read the line word by word
		while(word!=NULL)
		{
            // You can print the words of the inpit file for Debug purposes, just to make sure you are loading the input text as intended
			//printf("%s\n",word);

            
            // HINT: Since this nested while loop will keep reading the input text word by word, here is a good place to check for misspelled words
            
            
            // INPUT/OUTPUT SPECS: use the following line for printing a "word" that is misspelled.
            //printf("Misspelled word: %s\n",word);
            
            // INPUT/OUTPUT SPECS: use the following line for printing suggestions, each of which will be separated by a comma and whitespace.
            //printf("Suggestions: "); //the suggested words should follow
            
            
            
			word = strtok(NULL,delimiter); 
		}
	}
	fclose(fp);
    
    //HINT: If the flag noTypo is not altered (which you should do in the loop above if there exists a word not in the dictionary), then you should print "No typo!"
    if(noTypo==1)
        printf("No typo!\n");
    

    // DON'T FORGET to free the memory that you allocated
    freeHashTable(&hashTable);
    free(line);
	return 0;
}
