CC = gcc
CFLAGS = -Wall

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
	./$(TARGET)

runVal: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)

.PHONY: run runVal clean

