#include <mpi.h>
#include <stdio.h>
#include <thread>

#define ROOT 0
#define MSG_TAG 100
#define PENSIONERS_NR 20
#define CLUB_SIZE 50
#define ENTRY_AMOUNT 100

int main(int argc, char **argv)
{
	int size, tid;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &tid);

    Pensioner pensioner = new Pensioner(CLUB_SIZE); 

    przydzial_kwoty()

    MPI_Send(&i, 1, MPI_INT, (tid+1), MSG_TAG, MPI_COMM_WORLD);
    printf("# Send %d to %d\n", i, tid + 1);
    MPI_Recv(&i, 1, MPI_INT, (size-1), MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("# Reciv number %d from %d\n", i, status.MPI_SOURCE);
    printf("# FINISH!!!!\n");

    MPI_Recv(&i, 1, MPI_INT, tid-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("### Reciv number %d from %d\n", i, status.MPI_SOURCE);
    i += 1;
    MPI_Send(&i, 1, MPI_INT, ((tid+1)%size), MSG_TAG, MPI_COMM_WORLD);
    printf("### Send %d to %d\n", i, ((tid+1)%size));
	

	MPI_Finalize();
}
