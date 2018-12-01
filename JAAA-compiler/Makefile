GCC = gcc
GCCFLAGS = -g -Wall -pedantic -lm
CSOURCES   = $(wildcard *.c)

compiler: 
	yacc -d grammar.y 
	flex scanner.l
	$(GCC) $(GCCFLAGS) y.tab.c lex.yy.c $(CSOURCES) -o compiler -ly 
clean:
	rm compiler y.tab.c y.tab.h lex.yy.c
.PHONY:
	clean
