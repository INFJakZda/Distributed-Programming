#include "Pensioner.hpp"

#define PENSIONERS_NR 20
#define CLUB_SIZE 50
#define ENTRY_AMOUNT 100

int main() {

	srand(time(NULL));

	int size, rank;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	Pensioer pensioner = Pensioner(size, rank);
	pensioner.main_loop();

	MPI_Finalize();

	return 0;
}