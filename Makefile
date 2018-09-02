all: program
program: algorithm.c
	mpicc -lpthread -Wall algorithm.c -o program.exe

run:
	mpirun --hostfile hostfile -np 20 ./program.exe 4 100

clean:
	rm -f *.o program.exe
