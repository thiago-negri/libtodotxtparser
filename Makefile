CC := gcc

CFLAGS := $(shell cat compile_flags.txt)

HDR_DIR := header
SRC_DIR := src
OBJ_DIR := obj
HEADERS := $(shell find $(HDR_DIR) -type f -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: default
default: test

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -f libtodotxtparser.h
	rm -f libtodotxtparser.c

.PHONY: test
test: $(OBJECTS)
	./test.sh

.PHONY: amalgamation
amalgamation: libtodotxtparser.h libtodotxtparser.c

.PHONY: test-amalgamation
test-amalgamation: amalgamation
	./test.sh amalgamation

libtodotxtparser.h: $(HEADERS) amalgamation.sh
	./amalgamation.sh header

libtodotxtparser.c: $(SOURCES) amalgamation.sh
	./amalgamation.sh source

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -O -o $@ $<
