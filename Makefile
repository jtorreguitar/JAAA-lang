GCC = gcc
GCCFLAGS = -g -Wall -pedantic -lm
CSOURCES   = $(wildcard *.c)

compiler: 
	yacc -d grammar.y 
	flex scanner.l
	$(GCC) -o compiler lex.yy.c y.tab.c $(CSOURCES) -ly -ll $(GCCFLAGS)
clean:
	rm compiler y.tab.c y.tab.h lex.yy.c
