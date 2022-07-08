all: js out.txt
	./mdp/interpretador < out.txt

out.txt: zimbrao entradas/entrada.txt
	./zimbrao < entradas/entrada.txt > out.txt

lex.yy.c: zimbrao.l
	lex zimbrao.l

y.tab.c: zimbrao.y
	yacc zimbrao.y -Wall -v

js: lex.yy.c y.tab.c
	g++ -o zimbrao y.tab.c -ll

clr:
	rm -rf *.c *.output out.txt zimbrao
	clear

yacc:
	yacc zimbrao.y -Wall -v