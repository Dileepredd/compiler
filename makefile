minicc: y.tab.c lex.yy.c
	g++ y.tab.c lex.yy.c
lex.yy.c: lex.l
	lex lex.l
y.tab.c: parser.y
	yacc -v -d parser.y
clean:
	rm -f a.out y.tab.c lex.yy.c y.tab.h
run:
	./a.out test.txt