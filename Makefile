CC := gcc

CFLAGS := $(shell cat compile_flags.txt)

SRC_DIR := src
OBJ_DIR := obj
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -O -o $@ $<

.PHONY: test
test: $(OBJECTS)
	$(CC) $(CFLAGS) -g test.c $^ -o test
	./test
