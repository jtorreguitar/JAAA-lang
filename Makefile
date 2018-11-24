GCC = gcc
GCCFLAGS = -g -Wall -pedantic -lm

compiler: 
	yacc -d grammar.y 
	flex scanner.l
	$(GCC) -o compiler lex.yy.c y.tab.c node.c node.h listADT.c listADT.h -ly -ll $(GCCFLAGS)

clean:
	rm compiler y.tab.c y.tab.h lex.yy.c
