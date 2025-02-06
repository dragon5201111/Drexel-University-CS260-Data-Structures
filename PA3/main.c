#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

float getLoadFactor(HashTable * hashTable){
    return hashTable->size / hashTable->capacity;
}

int getHashKey(HashTable *hashTable, char *s) {
    int n = strlen(s);
    unsigned long hash = 0;

    for (int i = 0; i < n; i++) {
        unsigned long power = 1;
        for (int j = 0; j < n - i - 1; j++) {
            power *= 31;
        }
        hash += s[i] * power;
    }

    return hash % hashTable->capacity;
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

    for (int i = 0; i < capacity; i++) {
        hashTable->buckets[i] = (LinkedList *)malloc(sizeof(LinkedList));
        if (hashTable->buckets[i] == NULL) {
            printf("Unable to initialize bucket %d.\n", i);
            return;
        }
        hashTable->buckets[i]->head = NULL;
    }
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

void resizeHashTable(HashTable * hashTable){
    
}

void insertHashTable(HashTable * hashTable, char * key){
    if(hashTable == NULL || key == NULL ){
        printf("Key or hashtable is null. Unable to insert\n");
        return;
    }
    
    if(getLoadFactor(&hashTable) > DEFAULT_LOAD_FACTOR){

    }

    Node * node = createNode(key);

    if(node == NULL){
        printf("Unable to create node. Insertion failed.\n");
        return;
    }
 
    LinkedList * bucket = hashTable->buckets[getHashKey(hashTable, key)];
    Node * head = bucket->head;

    // Empty linked list
    if(head == NULL){
        bucket->head = node;
        return;
    }

    node->next = bucket->head;
    bucket->head = node;

    hashTable->size++;
}

int main(int argc, char **argv)
{
	HashTable ht;
    initializeHashTable(&ht, 16);
    insertHashTable(&ht, "foo");
    freeHashTable(&ht);
	return 0;
}
