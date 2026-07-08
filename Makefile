CFLAGS := $(shell cat compile_flags.txt)

.PHONY: test
test:
	gcc $(CFLAGS) -g test.c libtodotxtparser.c -o test
	./test
