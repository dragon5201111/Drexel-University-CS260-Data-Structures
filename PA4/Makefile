CC = gcc
CFLAGS = -Wall -g
INPUT_FILE = message.txt
ENCODE_DECODE = encode
OUTPUT_CODE_TABLE_FILE = ./out/test_code_table.txt
OUTPUT_CODE_TABLE_FILE_SOLUTION = ./out/test_code_table_solution.txt
ENCODED_DECODED_FILE = ./out/test_enc_dec.txt
ENCODED_DECODED_FILE_SOLUTIUON = ./out/test_enc_dec_solution.txt


SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = main
SOL = huffman

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET) $(ENCODE_DECODE) $(INPUT_FILE) $(OUTPUT_CODE_TABLE_FILE) $(ENCODED_DECODED_FILE)

	
runSol:
	./$(SOL) $(ENCODE_DECODE) $(INPUT_FILE) $(OUTPUT_CODE_TABLE_FILE_SOLUTION) $(ENCODED_DECODED_FILE_SOLUTIUON)

valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) $(ENCODE_DECODE) $(INPUT_FILE) $(OUTPUT_CODE_TABLE_FILE) $(ENCODED_DECODED_FILE)
	
.PHONY: clean run valgrind

