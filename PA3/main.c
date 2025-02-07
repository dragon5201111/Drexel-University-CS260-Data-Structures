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

void initializeBuckets(LinkedList ** buckets, int capacity){
    for (int i = 0; i < capacity; i++) {
        buckets[i] = (LinkedList *)malloc(sizeof(LinkedList));
        if (buckets[i] == NULL) {
            printf("Unable to initialize bucket %d.\n", i);
            return;
        }
        buckets[i]->head = NULL; // Make sure the bucket is initialized properly
    }
}

float getLoadFactor(HashTable * hashTable){
    return (float)hashTable->size / hashTable->capacity;
}

int getHash(char *s) {
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
    return getHash(s) % capacity;
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

    initializeBuckets(hashTable->buckets, capacity);
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

Node * createNode(char *key) {
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

void resizeHashTable(HashTable *hashTable) {
    int newCapacity = hashTable->capacity * 2;
    LinkedList **newBuckets = (LinkedList **)malloc(sizeof(LinkedList *) * newCapacity);

    if (newBuckets == NULL) {
        printf("Unable to create new buckets for resizing.\n");
        return;
    }

    initializeBuckets(newBuckets, newCapacity);

    Node *tempNode, *currentNode;

    // Rehash buckets
    for (int i = 0; i < hashTable->capacity; i++) {
        LinkedList *currentBucket = hashTable->buckets[i];
        currentNode = currentBucket->head;

        while (currentNode != NULL) {
            int hashKey = getHashKey(currentNode->key, newCapacity);

            Node *nodeCopy = createNode(currentNode->key);
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
    
    if(getLoadFactor(hashTable) > DEFAULT_LOAD_FACTOR){
        resizeHashTable(hashTable);
    }

    Node * node = createNode(key);

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


int main(int argc, char **argv) {
    HashTable ht;
    initializeHashTable(&ht, 16);
    
    freeHashTable(&ht);
    return 0;
}