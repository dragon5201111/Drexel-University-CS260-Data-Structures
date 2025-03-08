#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SlidingPuzzle{
	int k;
	int * board;
	struct SlidingPuzzle * predecessor_puzzle;
}SlidingPuzzle;

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
    if (puzzle == NULL){
        return;
	}

	printf("======================================\n");
    printf("Puzzle Statistics:\n======================================\n* Size -> %d x %d\n* Has Predecessor -> %s\n* Is Solvable -> %s\n", 
        puzzle->k, puzzle->k, 
        (puzzle->predecessor_puzzle == NULL) ? "No (NULL)" : "Yes",
		(!is_puzzle_unsolvable(puzzle)) ? "Yes" : "No");

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
	printf("======================================\n");
	printf("Zero Neighbors:\n");
	printf("======================================\n");

	int zero_neighbors [4];
	get_puzzle_neighbors(puzzle, zero_index, zero_neighbors);
	
	printf("Up Neighbor[%d] = \033[38;5;214m%d\033[0m\n", zero_neighbors[0], (zero_neighbors[0] == -1) ? -1 : puzzle->board[zero_neighbors[0]]);
	printf("Down Neighbor[%d] = \033[38;5;214m%d\033[0m\n", zero_neighbors[1], (zero_neighbors[1] == -1) ? -1 : puzzle->board[zero_neighbors[1]]);
	printf("Left Neighbor[%d] = \033[38;5;214m%d\033[0m\n", zero_neighbors[2], (zero_neighbors[2] == -1) ? -1 : puzzle->board[zero_neighbors[2]]);
	printf("Right Neighbor[%d] = \033[38;5;214m%d\033[0m", zero_neighbors[3], (zero_neighbors[3] == -1) ? -1 : puzzle->board[zero_neighbors[3]]);
	printf("\n======================================\n");
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
