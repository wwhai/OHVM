ODIR=obj
cc = gcc
objects = cpu.o clog.o main.o types.o utils.o ohvm.o graphics.o
ohvm : ${objects}
	 ${cc} -o ohvm ${objects}

.PHONY : clean
clean :
	-rm ohvm ${objects} *.ohvmbc
