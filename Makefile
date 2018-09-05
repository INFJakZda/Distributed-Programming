all: program
program: help.o leader.o mainLoop.o threadFunc.o
	mpicc -lpthread -Wall help.o mainLoop.o threadFunc.o leader.o main.c -o program.exe

help.o: help.c
	mpicc -c -Wall help.c -o help.o

leader.o: leader.c
	mpicc -c -Wall leader.c -o leader.o

mainLoop.o: mainLoop.c
	mpicc -c -Wall mainLoop.c -o mainLoop.o

threadFunc.o: threadFunc.c
	mpicc -c -Wall threadFunc.c -o threadFunc.o

run:
	mpirun --hostfile hostfile -np 20 ./program.exe 4 100

clean:
	rm -f *.o program.exe
