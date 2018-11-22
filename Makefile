GCC = gcc
GCCFLAGS = -Wall -pedantic -lm
DEBUGFLAGS = -g -Wall -pedantic -lm

compiler: 
	yacc -d grammar.y 
	flex scanner.l
	$(GCC) -o compiler lex.yy.c y.tab.c node.c node.h listADT.c listADT.h -ly -ll $(GCCFLAGS)

debug:
	yacc -d grammar.y 
	flex scanner.l
	$(GCC) -o compiler lex.yy.c y.tab.c node.c node.h listADT.c listADT.h -ly -ll $(DEBUGFLAGS)

clean:
	rm compiler y.tab.c y.tab.h lex.yy.c
