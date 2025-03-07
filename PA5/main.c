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

int is_puzzle_queue_empty(PuzzleQueue * puzzle_queue){
	return (puzzle_queue->head == NULL && puzzle_queue->tail == NULL);
}

PuzzleQueue * create_puzzle_queue(){
	PuzzleQueue * puzzle_queue;

	if((puzzle_queue = (PuzzleQueue*)malloc(sizeof(PuzzleQueue))) == NULL){
		return NULL;
	}
	
	puzzle_queue->head = puzzle_queue->tail = NULL;
	return puzzle_queue;
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

    if (puzzle->predecessor_puzzle != NULL){
        free_puzzle(puzzle->predecessor_puzzle);
	}

	if(puzzle->board != NULL){
    	free(puzzle->board);
	}

    free(puzzle);
}

int open_input_and_output_file(FILE ** input_file, FILE ** output_file, char ** argv){
	*input_file = fopen(argv[1], "r");
	*output_file = fopen(argv[2], "w");
	return (*input_file != NULL || *output_file != NULL);
}

void close_input_and_output_file(FILE * input_file, FILE * output_file){
	fclose(input_file);
	fclose(output_file);
}

void _print_puzzle(SlidingPuzzle *puzzle) {
    if (puzzle == NULL)
        return;
	printf("======================================\n");
    printf("Puzzle Statistics:\n* Size -> %d x %d\n* Predecessor -> %s\n", 
        puzzle->k, puzzle->k, 
        (puzzle->predecessor_puzzle == NULL) ? "No" : "Yes");
    
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
	printf("======================================\n");

}

int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "Requires an input and output file. Cannot proceed.\n");
		return EXIT_FAILURE;
	}

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

	_print_puzzle(initial_puzzle);

	/*
	TODO:
		0.) Add puzzle indexing operations
		1.) Implement puzzle queue
		2.) Implement BFS
		3.) Output solution
	*/

	free_puzzle(initial_puzzle);
	close_input_and_output_file(input_file, output_file);
	return 0;
}
