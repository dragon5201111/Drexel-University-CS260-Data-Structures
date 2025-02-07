CC = gcc
CFLAGS = -Wall
WORDS_FILE = words.txt
TEST_FILE = test.txt
ADD = add

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = main

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET) $(WORDS_FILE) $(TEST_FILE) $(ADD)

runVal: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) $(WORDS_FILE) $(TEST_FILE) $(ADD)

.PHONY: run runVal clean

