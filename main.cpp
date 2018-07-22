#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pthread.h>

/* BOOLEAN VALUES */
#define FALSE 0
#define TRUE 1

/* PROCES STATUS */
#define PROC_STATUS_ALONE 0
#define PROC_STATUS_LEADER 1
#define PROC_STATUS_MEMBER 2
#define PROC_STATUS_ACCEPT_INVITATION 3
#define PROC_STATUS_REJECT_INVITATION 4
#define PROC_STATUS_ENOUGH_MONEY 5
#define PROC_STATUS_REBOOT 6

/* MESSAGE STATUS */
#define MESSAGE_STATUS_

/* ARRAY VALUES */
#define ARRAY_VAL_NOT_ASKED 0
#define ARRAY_VAL_ASKED_ACCEPT 1
#define ARRAY_VAL_ASKED_REJECT 2

/* GLOBAL VARIABLES (ONLY FOR THREADS) */
int N;
int M;
int K;
int RANK;

int main(int argc, char **argv)
{
	K = atoi(argv[1]);
	M = atoi(argv[2]);
	
	MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
	
	while(TRUE)
	{
		
	}
	
	return 0;
}
