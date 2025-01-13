CC = gcc
CFLAGS = -c

TARGET_RUN = run

OBJ = quickSort.o common.o

$(TARGET_RUN): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	-rm *.o
	-rm $(TARGET_RUN)

.PHONY: clean
