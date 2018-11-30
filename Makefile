GCC = gcc
GCCFLAGS = -g -Wall -pedantic -lm
CSOURCES   = $(wildcard *.c)

compiler:
	make -C JAAA-compiler/ compiler
clean:
	make -C JAAA-compiler/ clean
.PHONY:
	clean	
