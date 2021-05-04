cc = gcc
objects = lex.yy.o y.tab.o
target = ohvmlang
ohvmlang : ${objects}
	 ${cc} -o ${target} ${objects}

.PHONY : clean
clean :
	-rm ${target} ${objects}
