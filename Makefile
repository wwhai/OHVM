CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

ifeq ($(OS),Windows_NT)
CLEAN = del /Q ohvm.exe test_vm.exe examples\make_examples.exe *.o examples\*.ohvmbc 2>NUL
else
CLEAN = rm -f ohvm test_vm examples/make_examples *.o examples/*.ohvmbc
endif

.PHONY: all examples test clean

all: ohvm

ohvm: main.o vm.o
	$(CC) $(CFLAGS) -o $@ $^

test_vm: test_vm.o vm.o
	$(CC) $(CFLAGS) -o $@ $^

test: test_vm
	./test_vm

examples: examples/make_examples
	./examples/make_examples

examples/make_examples: examples/make_examples.c vm.h
	$(CC) $(CFLAGS) -o $@ examples/make_examples.c

clean:
	-$(CLEAN)
