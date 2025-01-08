// PROGRAMMING ASSIGNMENT I STARTER CODE
/*In order to complete this assignment, you need to complete the function defintions in the section commented FUNCTION DEFINITIONS.
When you implement a function, make sure that you uncomment its provided function call from main so that your program can utilize it.*/

// HEADER FILES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// CONSTANT DECLARATIONS
#define INITIAL_CAPACITY 2

// STRUCT DECLARATIONS
typedef struct entry
{
	char* name;
	char* lastname;
	float height;
	int age;
} Entry;

typedef struct list 
{
	int capacity;
	int size;
	Entry** data;
} List;

// FUNCTION DEFINTIONS
/* The function declarations have been given to you which should make the parameters and the return value for each function obvious.
The initializeEntry, freeEntry, printList, and printEntry function have been coded to start you off.*/

// Returns a pointer to an Entry struct initialized with the specified values for each of the 4 members of the struct (NO NEED TO CHANGE).
Entry* initializeEntry(char* name, char* lastname, float height, int age)
{
	Entry* newEntry = malloc(sizeof(Entry));
	
	newEntry->name = malloc(sizeof(char)*(strlen(name)+1));
	strcpy(newEntry->name, name);
	
	newEntry->lastname = malloc(sizeof(char)*(strlen(lastname)+1));
	strcpy(newEntry->lastname, lastname);
	
	newEntry->height = height;
	
	newEntry->age = age;
	
	return newEntry;
}

// Given a pointer to an Entry struct, this function frees the memory allocated for that Entry struct (NO NEED TO CHANGE).
void freeEntry(Entry* entry)
{
	if (entry != NULL)
	{
		free(entry->name);
		free(entry->lastname);
		free(entry);
	}
}

List * initializeList(){
	List * newList = (List *) malloc(sizeof(List));
	
	if(newList == NULL){
		printf("Unable to allocate memory for list.\n");
		return NULL;
	}

	newList->size = 0;
	newList->capacity = INITIAL_CAPACITY;
	newList->data = (Entry **) malloc(sizeof(Entry *) * INITIAL_CAPACITY);
	
	if(newList->data == NULL){
		printf("Unable to allocate memory for entries.\n");
		free(newList);
		return NULL;
	}

	return newList;
}

void deleteList(List * myList){
	if(myList == NULL){
		printf("No list to delete!\n");
		return;
	}
	
	if(myList->data != NULL){
		for(size_t i = 0; i < myList->size; i++){
			freeEntry(myList->data[i]);
		}

		free(myList->data);
	}

	free(myList);
}

int setCapacity(List * myList, int cap){
	if(myList == NULL || myList->data == NULL){
		printf("No list to set capacity on!\n");
		return -1;
	}

	if(cap <= 0){
		printf("Capacity must be greater than 0.\n");
		return -2;
	}

	Entry ** newBlock = (Entry **) realloc(myList->data, sizeof(Entry *) * cap);

    if (newBlock == NULL) {
        printf("Failed to set capacity. Memory reallocation failed.\n");
        return -3;
    }

    myList->data = newBlock;
    myList->capacity = cap;

	return 0;
}

void doubleCapacity(List * myList){
	if(setCapacity(myList, myList->capacity * 2) < 0){
		printf("Unable to double capacity of list!\n");
		return;
	}
}

void halveCapacity(List * myList){
	if(setCapacity(myList, myList->capacity / 2) < 0){
		printf("Unable to halve capacity of list!\n");
		return;
	}
}

void insertToTail(List* myList, char* name, char* lastname, float height, int age){
	if(myList == NULL){
		printf("No list to insert onto tail.\n");
		return;
	}

	if(myList->size == myList->capacity){
		doubleCapacity(myList);
	}

	myList->data[myList->size++] = initializeEntry(name, lastname, height, age);
}

void insertToHead(List* myList, char* name, char* lastname, float height, int age){
	if(myList == NULL){
		printf("No list to insert before head.\n");
		return;
	}

	if(myList->size == myList->capacity){
		doubleCapacity(myList);
	}

	if(myList->size == 0){
		insertToTail(myList, name, lastname, height, age);
	}else{
		for(int i = myList->size - 1; i >= 0; i--) {
    		myList->data[i + 1] = myList->data[i];
		}

		myList->data[0] = initializeEntry(name, lastname, height, age);
		myList->size++;
	}

}

void deleteFromTail(List* myList){
	if(myList == NULL){
		printf("No list to delete from.\n");
		return;
	}

	if(myList->size == 0){
		printf("No tail to delete from!\n");
		return;
	}

	freeEntry(myList->data[myList->size - 1]);
	myList->data[myList->size - 1] = NULL;
	myList->size--;

	if(myList->size < myList->capacity/2){
		halveCapacity(myList);
	}
}

void deleteFromHead(List * myList){
	if(myList == NULL){
		printf("No list to delete from.\n");
		return;
	}

	if(myList->size == 0){
		printf("No head to delete from!\n");
		return;
	}

	freeEntry(myList->data[0]);
	myList->data[0] = NULL;

	for (int i = 1; i < myList->size; i++)
	{
		myList->data[i - 1] = myList->data[i];
	}

	myList->size--;

	if(myList->size < myList->capacity/2){
		halveCapacity(myList);
	}
}

int findPosition(List* myList, char* name){
	if(myList == NULL){
		printf("No list to search!\n");
		return -1;
	}

	for(int i = 0; i < myList->size; i++){
		if(strcmp(name, myList->data[i]->name) == 0){
			return i;
		}
	}

	return -1;
}

void insertToPosition(List* myList, int position, char* name, char* lastname, float height, int age){
	if(myList == NULL){
			printf("No list to insert into.\n");
			return;
	}
	
	if(position >=0 && position <= myList->size){

		if(myList->size == myList->capacity){
			doubleCapacity(myList);
		}
		
		for(int i = myList->size - 1; i >= position; i--) {
    		myList->data[i + 1] = myList->data[i];
		}

		myList->data[position] = initializeEntry(name, lastname, height, age);
		myList->size++;

	}else{
		printf("Cannot insert at position: %d\n. Invalid.", position);
	}
}

List* initializeList();

int setCapacity(List * myList, int cap);
void deleteList(List* myList);

void doubleCapacity(List* myList);

void halveCapacity(List* myList);

void insertToHead(List* myList, char* name, char* lastname, float height, int age);

void insertToTail(List* myList, char* name, char* lastname, float height, int age);

void insertToPosition(List* myList, int position, char* name, char* lastname, float height, int age);

int findPosition(List* myList, char* name);

void deleteFromHead(List* myList);

void deleteFromTail(List* myList);

void deleteFromPosition(List* myList, int position);

// Given a pointer to a List struct, this function prints each Entry in that list (NO NEED TO CHANGE).
void printList(List* myList)
{
	if (myList->size == 0)
	{
		printf("List is empty!\n");
	}
	else
	{
		for (int i = 0; i < myList->size; i++)
		{
			printf("[%d]\t%s\t%s\t%0.2f\t%d\n", i, myList->data[i]->name, myList->data[i]->lastname, myList->data[i]->height, myList->data[i]->age);
		}
	}
}

// Given a pointer to a List struct, this function prints out the size and capacity of that List (NO NEED TO CHANGE).
void printListInfo(List* myList)
{
	printf("size:%d, capacity:%d\n", myList->size, myList->capacity);
}

int main(int argc, char** argv) 
{
	FILE* fp = fopen(argv[1], "r");
	char* line = NULL;
	size_t lineBuffSize = 0;
	ssize_t lineSize;
	
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}
	
	List* myList;
	// Uncomment the following function call when you implement the initializeList() function
	myList = initializeList(); 
	
	while ((lineSize = getline(&line, &lineBuffSize, fp)) != -1)
	{
		char* token;
		const char delimiter[2] = " ";
		
		if (line[strlen(line) - 1] == '\n')
		{
			line[strlen(line) - 1] = '\0';
		}

		token = strtok(line, delimiter);
					
		if (strcmp(token, "insertToHead") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			// Uncomment the following insertToHead function call when you have implemented it
			insertToHead(myList, name, lastname, height, age);
		}
		else if (strcmp(token, "insertToTail") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			// Uncomment the following insertToTail function call when you have implemented it
			insertToTail(myList, name, lastname, height, age);
		}
		else if (strcmp(token, "insertToPosition") == 0)
		{
			char* name;
			char* lastname;
			float height;
			int age;
			int position;
			
			position = atoi(strtok(NULL, delimiter));
			name = strtok(NULL, delimiter);
			lastname = strtok(NULL, delimiter);
			height = atof(strtok(NULL, delimiter));
			age = atoi(strtok(NULL, delimiter));
			// Uncomment the following insertToPosition function call when you have implemented it
			insertToPosition(myList, position, name, lastname, height, age);
		}
		else if (strcmp(token, "findPosition") == 0)
		{
			char* name;
			
			name = strtok(NULL, delimiter);
			// Uncomment the following printf statement using the return value from a findPosition function call when you have implemented the function
			printf("%d\n", findPosition(myList, name));
		}
		else if (strcmp(token, "deleteFromHead") == 0)
		{
			// Uncomment the following deleteFromHead function call when you have implemented it
			deleteFromHead(myList);
		}
		else if (strcmp(token, "deleteFromTail") == 0)
		{
			// Uncomment the following insertToHead function call when you have implemented it
			deleteFromTail(myList);
		}
		else if (strcmp(token, "deleteFromPosition") == 0)
		{
			int position;
			
			position = atoi(strtok(NULL, delimiter));
			
			// Uncomment the following deleteFromPosition function call when you have implemented it
			// deleteFromPosition(myList, position);
		}
		else if (strcmp(token, "printList") == 0)
		{
			printList(myList);
		}
		else if(strcmp(token, "printListInfo") == 0)
		{
			printListInfo(myList);
		}
		else if (strcmp(token, "deleteList") == 0)
		{
			// Uncomment the following deleteList function call when you have implemented it
			deleteList(myList);
		}
		else
		{
			printf("Invalid command: <%s>\n", token);
		}
	}

	//getline internally allocates memory, so we need to free it here so as not to leak memory!!
	free(line);
	
	//we should close the file that we have been reading from before exiting!
	fclose(fp);

	return 0;
} 
