.PHONY:clean runVal

CC=gcc
CFLAGS=-Wall
SOURCES=$(wildcard *.c)
OBJS = $(SOURCES:.c=.o)
EXE=run

$(EXE):$(OBJS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

$(OBJS):$(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)

runVal : $(EXE)
	valgrind --leak-check=yes ./$(EXE)

clean:
	-rm $(OBJS) $(EXE)

