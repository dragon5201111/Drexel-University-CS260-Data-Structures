CC=gcc
CFLAGS = -Wall -g
SRC=$(wildcard *.c)
OBJ = $(SRC:.c=.o)

FILE_NAME=0_easy
FILE_EXT=.txt
IN_FILE="./test_input/$(FILE_NAME)$(FILE_EXT)"
OUT_FILE="./test_output/$(FILE_NAME)_out$(FILE_EXT)"

TARGET=main
TARGET_SOLUTION=solution

# $@ = target, $^ = pre-reqs
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# $< = first pre-req
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(IN_FILE) $(OUT_FILE)

runSolution: $(TARGET_SOLUTION)
	./$(TARGET_SOLUTION) $(IN_FILE) $(OUT_FILE)

valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) $(IN_FILE) $(OUT_FILE)

clean:
	rm -rf *.o $(TARGET)

.PHONY: run runSolution clean valgrind