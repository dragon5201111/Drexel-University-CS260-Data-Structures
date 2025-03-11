CC=gcc
CFLAGS = -Wall -g
SRC=$(wildcard *.c)
OBJ = $(SRC:.c=.o)

FILE_NAME=3_hard
FILE_EXT=.txt
IN_FILE="./test_input/$(FILE_NAME)$(FILE_EXT)"
OUT_FILE="./test_output/$(FILE_NAME)_out$(FILE_EXT)"
OUT_FILE_SOLUTION="./test_output/$(FILE_NAME)_out_s$(FILE_EXT)"

TARGET=main
TARGET_SOLUTION=solution

# $@ = target, $^ = pre-reqs
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# $< = first pre-req
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(IN_FILE) $(OUT_FILE)

runSolution: $(TARGET_SOLUTION)
	./$(TARGET_SOLUTION) $(IN_FILE) $(OUT_FILE_SOLUTION)

valgrind: $(TARGET)
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(TARGET) $(IN_FILE) $(OUT_FILE)

clean:
	rm -rf *.o $(TARGET)

.PHONY: run runSolution clean valgrind