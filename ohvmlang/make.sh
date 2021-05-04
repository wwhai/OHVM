rm ./*.o
lex ohvmlang.L
yacc -d ohvmlang.Y
cc -c y.tab.c lex.yy.c