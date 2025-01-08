IN_FILE = input3.txt

main: main.o
	gcc main.o -o main
main.o: main.c
	gcc -c main.c
clean:
	-rm main
	-rm *.o
run: main
	./main

runVal: main
	valgrind --tool=memcheck --leak-check=yes ./main $(IN_FILE)

.PHONY: clean run runVal