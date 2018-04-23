#include "main.hpp"

int main(int argc, char **argv)
{
	int size, rank;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Pensioner pensioner = new Pensioner();

    pensioner.set_money_amount();  

	MPI_Finalize();
}
