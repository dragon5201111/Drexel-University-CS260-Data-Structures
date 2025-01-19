DICT_FILE = dict.txt
QUERY_FILE = queries_1.txt

main: main.o
	gcc main.o -o main
main.o: main.c
	gcc -c main.c
clean:
	-rm main
	-rm *.o
run: main
	./main $(DICT_FILE) $(QUERY_FILE)

runVal: main
	valgrind --tool=memcheck --leak-check=yes ./main $(DICT_FILE) $(QUERY_FILE)

runTime: main
	time ./main $(DICT_FILE) $(QUERY_FILE)

.PHONY: clean run runVal