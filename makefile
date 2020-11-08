minicc: ./yacc/y.tab.c ./lex/lex.yy.c
	g++ y.tab.c lex.yy.c ./codegen/compile.cpp ./codegen/ast.cpp
./lex/lex.yy.c: ./lex/lex.l
	lex ./lex/lex.l
./yacc/y.tab.c: ./yacc/parser.y
	yacc -v -d ./yacc/parser.y
clean:
	rm -f a.out y.tab.c lex.yy.c y.tab.h y.output
run:
	./a.out ./test/test.txt