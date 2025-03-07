CC=gcc
CFLAGS = -Wall -g
SRC=$(wildcard *.c)
OBJ = $(SRC:.c=.o)

IN_FILE="test_input/3_easy.txt"
OUT_FILE="test_output/3_easy_out.txt"

TARGET=main

# $@ = target, $^ = pre-reqs
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# $< = first pre-req
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(IN_FILE) $(OUT_FILE)

clean:
	rm -rf *.o $(TARGET)

.PHONY: run clean