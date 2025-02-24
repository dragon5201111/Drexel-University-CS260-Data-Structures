CC = gcc
CFLAGS = -Wall -g


SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = main
SOL = huffman

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(TARGET) $(OBJ)
	-rm -rf ./out/*

runEncode: $(TARGET)
	./$(TARGET) encode ./out/message.txt ./out/code_table.txt ./out/encoded.txt

runDecode: $(TARGET)
	./$(TARGET) decode ./out/code_table.txt ./out/encoded.txt ./out/decoded.txt

	
runEncodeSol:
	./$(SOL) encode ./out/message.txt ./out/code_table.txt ./out/encoded.txt

runDecodeSol:
	./$(SOL) decode ./out/code_table.txt ./out/encoded.txt ./out/decoded.txt

valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) decode ./out/code_table.txt ./out/encoded.txt ./out/decoded.txt
	
.PHONY: clean runEncode runDecode runDecodeSol runEncodeSol

