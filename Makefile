GCC = gcc
GCCFLAGS = -Wall -pedantic -lm

compiler: 
		yacc -d grammar.y 
		flex scanner.l
		$(GCC) -o compiler lex.yy.c y.tab.c node.c node.h listADT.c listADT.h -ly -ll $(GCCFLAGS)

clean:
	rm compiler y.tab.c y.tab.h y.tab.o lex.yy.o lex.yy.c