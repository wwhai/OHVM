cc = gcc
objects = cpu.o clog.o main.o types.o utils.o ohvm.o graphics.o
ohvm : ${objects}
	 ${cc} -o ohvm ${objects} -lSDL2 -lSDL2_ttf

.PHONY : clean
clean :
	-rm ohvm ohvmcc ${objects}
