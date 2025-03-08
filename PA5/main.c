#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SlidingPuzzle{
	int k;
	int * board;
	struct SlidingPuzzle * predecessor_puzzle;
}SlidingPuzzle;

typedef struct PuzzleQueueNode{
	SlidingPuzzle * puzzle;
	struct PuzzleQueueNode * next;
}PuzzleQueueNode;

typedef struct PuzzleQueue{
	PuzzleQueueNode * head;
	PuzzleQueueNode * tail;
}PuzzleQueue;

// Queue Functions
/*
* Creates a new PuzzleQueue.
* Returns: 
*     - PuzzleQueue pointer if the queue is successfully created
*     - NULL if memory allocation fails.
*/
PuzzleQueue * create_puzzle_queue() {
    PuzzleQueue * puzzle_queue;
    if((puzzle_queue = (PuzzleQueue *) malloc(sizeof(PuzzleQueue))) == NULL){
        return NULL;
    }
    puzzle_queue->head = puzzle_queue->tail = NULL;
    return puzzle_queue;
}

/*
* Creates a new PuzzleQueueNode.
* Assumes puzzle is non-NULL.
* Returns:
*     - PuzzleQueueNode pointer if successful, otherwise NULL.
*/
PuzzleQueueNode * create_puzzle_queue_node(SlidingPuzzle * puzzle) {
	if (puzzle == NULL) {
        return NULL;
    }

    PuzzleQueueNode * puzzle_queue_node;
    if((puzzle_queue_node = (PuzzleQueueNode *) malloc(sizeof(PuzzleQueueNode))) == NULL) {
        return NULL;
    }
    puzzle_queue_node->next = NULL;
    puzzle_queue_node->puzzle = puzzle;
    return puzzle_queue_node;
}

/*
Assumes puzzle_queue and puzzle_queue_node are non-NULL
Returns:
    - PuzzleQueue* on success, otherwise NULL
*/
PuzzleQueue * enqueue_puzzle_queue_node(PuzzleQueue * puzzle_queue, PuzzleQueueNode * puzzle_queue_node) {
    if (puzzle_queue == NULL || puzzle_queue_node == NULL) {
        return NULL; // Sanity check
    }

	if (puzzle_queue->head == NULL) {
        puzzle_queue->head = puzzle_queue->tail = puzzle_queue_node;
    } else {
        puzzle_queue->tail->next = puzzle_queue_node;
        puzzle_queue->tail = puzzle_queue_node;
    }
    return puzzle_queue;
}

/*
Assumes puzzle_queue is non-NULL
Returns:
    - PuzzleQueueNode* on success, NULL on failure
*/
PuzzleQueueNode * dequeue_puzzle_queue_node(PuzzleQueue * puzzle_queue){
	if (puzzle_queue == NULL || puzzle_queue->head == NULL) {
        return NULL;
    }
	
	PuzzleQueueNode * dequeued_node = puzzle_queue->head;
	puzzle_queue->head = puzzle_queue->head->next;

	if(puzzle_queue->head == NULL){
		puzzle_queue->tail = NULL;
	}

	return dequeued_node;
}

void free_puzzle_queue(PuzzleQueue * puzzle_queue) {
    PuzzleQueueNode * current = puzzle_queue->head;
    while (current != NULL) {
        PuzzleQueueNode * next_node = current->next;
        free(current);
        current = next_node;
    }
    free(puzzle_queue);
}

int puzzle_queue_is_empty(PuzzleQueue * puzzle_queue){
	return (puzzle_queue->head == NULL && puzzle_queue->tail == NULL);
}






// File Functions
int open_input_and_output_file(FILE ** input_file, FILE ** output_file, char ** argv){
	*input_file = fopen(argv[1], "r");
	*output_file = fopen(argv[2], "w");
	return (*input_file != NULL || *output_file != NULL);
}

void close_input_and_output_file(FILE * input_file, FILE * output_file){
	fclose(input_file);
	fclose(output_file);
}






// Puzzle Functions
/*
Returns:
	1 if cannot be solved, otherwise, 0
*/
int is_puzzle_unsolvable(SlidingPuzzle * puzzle) {
    int k = puzzle->k;
    int inverted_pairs = 0;
    int zero_index = -1;

    for (int i = 0; i < k * k; i++) {
        if (puzzle->board[i] == 0) {
            zero_index = i;
			break;
        }
    }

    for (int i = 0; i < k * k; i++) {
        for (int j = i + 1; j < k * k; j++) {
            if (puzzle->board[i] > puzzle->board[j] && 
				puzzle->board[i] != 0 && 
				puzzle->board[j] != 0) {
                inverted_pairs++;
            }
        }
    }

	// k is even
    if (k % 2 == 0) {
        int zero_row = zero_index / k;
		// Inversions plus zero_row is even
        return (inverted_pairs + zero_row) % 2 == 0;
    }

	// k is odd and number of inversions is odd
    return inverted_pairs % 2 != 0;
}

SlidingPuzzle * create_puzzle(int k, int * board, SlidingPuzzle * predecessor_puzzle){
	SlidingPuzzle * sliding_puzzle;
	if((sliding_puzzle = (SlidingPuzzle *)malloc(sizeof(SlidingPuzzle))) == NULL)
		return NULL;

	sliding_puzzle->k = k;
	sliding_puzzle->predecessor_puzzle = predecessor_puzzle;

	
	if ((sliding_puzzle->board = (int *) malloc(sizeof(int) * k * k)) == NULL) {
        free(sliding_puzzle);
        return NULL;
    }

	memcpy(sliding_puzzle->board, board, k * k * sizeof(int));
	return sliding_puzzle;
}

SlidingPuzzle * create_puzzle_from_input_file(FILE * input_file){
	char * line_buffer = NULL;
	int line_buffer_size = 0;
	int k = -1;
	
	// Skip first line
	if(getline(&line_buffer, (size_t *)&line_buffer_size, input_file) == -1)
		return NULL;

	fscanf(input_file, "%d\n", &k);

	// Skip next line
	if(getline(&line_buffer, (size_t *)&line_buffer_size, input_file) == -1)
		return NULL;

	int input_board[k * k];
	for(int i = 0; i < k * k; i++)
		fscanf(input_file,"%d ",&input_board[i]);

	free(line_buffer);
	return create_puzzle(k, input_board, NULL);
}

void free_puzzle(SlidingPuzzle *puzzle) {
    if (puzzle == NULL) return;

	if(puzzle->board != NULL){
    	free(puzzle->board);
	}

    free(puzzle);
}

int row_and_column_in_puzzle_bounds(int row, int column, int k){
	return (row >= 0 && column >=0 && row < k && column < k);
}

/*	
	First index is up, second is down, third is left, fourth is right
	[-1,-1,-1,-1]
	-1 at an index indicates there is no neighbor for that position respectively
*/
void get_puzzle_neighbors(SlidingPuzzle * puzzle, int i, int * neighbor_array){
	int k = puzzle->k;
	int row = i / k;
	int column = i % k;

	// up
	neighbor_array[0] = row_and_column_in_puzzle_bounds(row-1, column, k) ? (row - 1) * k + column : -1; 
	// down
	neighbor_array[1] = row_and_column_in_puzzle_bounds(row+1, column, k) ? (row + 1) * k + column : -1; 
	// left
	neighbor_array[2] = row_and_column_in_puzzle_bounds(row, column-1, k) ? row * k + (column - 1) : -1; 
	// right
	neighbor_array[3] = row_and_column_in_puzzle_bounds(row, column+1, k) ? row * k + (column + 1) : -1; 
}

void get_zero_neighbors(SlidingPuzzle * puzzle, int * neighbor_array){
	int zero_index = -1;
	int k = puzzle->k;

	for(int i = 0; i < k*k; i++){
		if(puzzle->board[i] == 0){
			zero_index = i;
			break;
		}
	}

	get_puzzle_neighbors(puzzle, zero_index, neighbor_array);
}

int puzzles_are_the_same(SlidingPuzzle * puzzle_one, SlidingPuzzle * puzzle_two){
	if(puzzle_one == NULL || puzzle_two == NULL) return 0;
	// Boards will not have different sizes so, k will suffice
	int k = puzzle_one->k;
	return memcmp(puzzle_one->board, puzzle_two->board, sizeof(int) * k * k) == 0;
}

void _print_puzzle_statistics(SlidingPuzzle *puzzle){
	printf("======================================\n");
    printf("Puzzle Statistics:\n======================================\n* Size -> %d x %d\n* Has Predecessor -> %s\n* Is Solvable -> %s\n", 
        puzzle->k, puzzle->k, 
        (puzzle->predecessor_puzzle == NULL) ? "No (NULL)" : "Yes",
		(!is_puzzle_unsolvable(puzzle)) ? "Yes" : "No");
}

void _print_puzzle_board(SlidingPuzzle *puzzle){
	if(puzzle == NULL){
		return;
	}

	printf("======================================\n");
	printf("Game Board:\n");
	printf("======================================\n");

    int k = puzzle->k;
    int zero_index = -1;
    
    for (int i = 0; i < k * k; i++) {
        if (puzzle->board[i] == 0) {
            zero_index = i;
            break;
        }
    }

    // Print the board
    for (int i = 0; i < k * k; i++) {
        int is_neighbor = 0;
        
        int row = i / k;
        int col = i % k;
        
		int zero_row = zero_index / k;
		int zero_col = zero_index % k;
		if ((row == zero_row && abs(col - zero_col) == 1) || 
			(col == zero_col && abs(row - zero_row) == 1)) {
			is_neighbor = 1;
		}

        if (puzzle->board[i] == 0) {
            printf("\033[31m%-3d\033[0m ", puzzle->board[i]); // print 0 in red
        } else if (is_neighbor) {
            printf("\033[38;5;214m%-3d\033[0m ", puzzle->board[i]); // print neighbor in orange
        } else {
            printf("%-3d ", puzzle->board[i]); // print other numbers in default color
        }

        if ((i + 1) % k == 0)
            putchar('\n');
    }
}

void _print_zero_neighbors(SlidingPuzzle *puzzle){
	printf("======================================\n");
	printf("Zero Neighbors:\n");
	printf("======================================\n");

	int zero_neighbors [4];
	get_zero_neighbors(puzzle, zero_neighbors);
	
	printf("Up Neighbor[%d] = \033[38;5;214m%d\033[0m\n", zero_neighbors[0], (zero_neighbors[0] == -1) ? -1 : puzzle->board[zero_neighbors[0]]);
	printf("Down Neighbor[%d] = \033[38;5;214m%d\033[0m\n", zero_neighbors[1], (zero_neighbors[1] == -1) ? -1 : puzzle->board[zero_neighbors[1]]);
	printf("Left Neighbor[%d] = \033[38;5;214m%d\033[0m\n", zero_neighbors[2], (zero_neighbors[2] == -1) ? -1 : puzzle->board[zero_neighbors[2]]);
	printf("Right Neighbor[%d] = \033[38;5;214m%d\033[0m", zero_neighbors[3], (zero_neighbors[3] == -1) ? -1 : puzzle->board[zero_neighbors[3]]);
	printf("\n======================================\n\n");
}

void _print_puzzle(SlidingPuzzle *puzzle) {
    if (puzzle == NULL){
        return;
	}

	_print_puzzle_statistics(puzzle);
    _print_puzzle_board(puzzle);
	_print_zero_neighbors(puzzle);
}







int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "Requires an input and output file. Cannot proceed.\n");
		return EXIT_FAILURE;
	}

	// Open input and output files
	FILE *input_file = NULL, *output_file = NULL;
	if(!open_input_and_output_file(&input_file, &output_file, argv)){
		fprintf(stderr, "Couldn't open input or output file.\n");
		return EXIT_FAILURE;
	}
	
	SlidingPuzzle * initial_puzzle;
	if((initial_puzzle = create_puzzle_from_input_file(input_file)) == NULL){
		fprintf(stderr, "Unable to create initial sliding puzzle.\n");
		close_input_and_output_file(input_file, output_file);
		return EXIT_FAILURE;
	}

	// Debug print
	_print_puzzle(initial_puzzle);

	// Create queue
	PuzzleQueue * puzzle_queue;
	if((puzzle_queue = create_puzzle_queue()) == NULL){
		fprintf(stderr, "Unable to allocate memory for puzzle queue.\n");
		free_puzzle(initial_puzzle);
		close_input_and_output_file(input_file, output_file);
		return EXIT_FAILURE;
	}

	/*
	TODO:
		1.) Implement Hash Table to keep track of created boards
		2.) Implement BFS
		2.5) Maybe implement adjacency list???
		3.) Output solution
	*/
	free_puzzle(initial_puzzle);
	free_puzzle_queue(puzzle_queue);
	close_input_and_output_file(input_file, output_file);
	return 0;
}
