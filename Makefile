cc = gcc
LIBS = sdl2
objects = cpu.o log.o main.o types.o utils.o vm.o execute_function.o
openvm : ${objects}
	${cc} -o openvm ${objects}
.PHONY : clean
clean :
	-rm openvm ${objects}
