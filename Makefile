cc = gcc
objects = cpu.o log.o main.o types.o utils.o vm.o
openvm : ${objects}
	${cc} -o openvm ${objects}
openvm-debug : ${objects}
	${cc} -g -o openvm ${objects}
.PHONY : clean
clean :
	-rm openvm ${objects}
