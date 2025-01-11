TARGET_C = quickSort.c
TARGET_N = run
GCC_ARGS = common.c

$(TARGET_N): $(TARGET_C)
	gcc $(GCC_ARGS) $(TARGET_C) -o $(TARGET_N)

clean:
	-rm $(TARGET_N)

.PHONY: clean
