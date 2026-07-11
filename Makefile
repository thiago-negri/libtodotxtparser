CC := gcc

CFLAGS := $(shell cat compile_flags.txt)

HDR_DIR := headers
SRC_DIR := src
OBJ_DIR := obj
HEADERS := $(shell find $(HDR_DIR) -type f -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: test
test: $(OBJECTS)
	$(CC) $(CFLAGS) -g -fsanitize=address test.c $^ -o test
	./test

.PHONY: amalgamation
amalgamation: libtodotxtparser.h libtodotxtparser.c

.PHONY: test-amalgamation
test-amalgamation: amalgamation
	$(CC) $(CFLAGS) -D__AMALGAMATION__ -g -fsanitize=address test.c libtodotxtparser.c -o test
	./test

libtodotxtparser.h: $(HEADERS) amalgamation.sh
	./amalgamation.sh header

libtodotxtparser.c: $(SOURCES) amalgamation.sh
	./amalgamation.sh source

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -O -o $@ $<
