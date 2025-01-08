main: main.o
	gcc main.o -o main
main.o: main.c
	gcc -c main.c
clean:
	-rm main
	-rm *.o
run: main
	./main
.PHONY: clean run