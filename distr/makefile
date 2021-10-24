checker: checker.o y.tab.o lex.yy.o errormsg.o util.o ast.o symbol.o table.o typecheck.o
	gcc -g -o checker checker.o y.tab.o lex.yy.o errormsg.o util.o ast.o symbol.o table.o typecheck.o

checker.o: checker.c errormsg.h util.h
	gcc -g -c checker.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

y.tab.c: parser.y
	 bison -o y.tab.c -dv parser.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c errormsg.h util.h
	gcc -g -c errormsg.c

symbol.o: symbol.c symbol.h
	gcc -g -c symbol.c

table.o: table.c table.h
	gcc -g -c table.c

typecheck.o: typecheck.c typecheck.h
	gcc -g -c typecheck.c

lex.yy.o: lex.yy.c y.tab.h errormsg.h util.h
	gcc -g -c lex.yy.c

lex.yy.c: lexer.lex
	flex lexer.lex

util.o: util.c util.h
	gcc -g -c util.c

ast.o: ast.c ast.h
	gcc -g -c ast.c

output: checker parser.y
	./$<
	for test in `ls testcases`; do \
	    echo "running test case $${test}"; \
		./$< testcases/$${test} > log; \
        test $${test} = "test4.prg" && diff log fail4.txt || (test $${test} = "test7.prg" && diff -a log fail7.txt || diff log success); \
	done

clean: 
	rm -f checker util.o checker.o lex.yy.o errormsg.o ast.o symbol.o table.o typecheck.o y.tab.c y.tab.h y.tab.o y.output
