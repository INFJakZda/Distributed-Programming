#include "main.hpp"

int main(int argc, char **argv)
{
	srand(time(NULL));

	int size, rank;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Pensioner pensioner = new Pensioner();

	while(true){
		pensioner.set_money_amount();  

		while(true){
			if(pensioner.check_if_leader()) {
				if(pensioner.get_group_money() > ENTRY_AMOUNT) {
					pensioner.choose_club();
					break;
				}
				else {
					pensioner.thread_communication();

				}
			}
			else {
				MPI_Recv();
				break;
			}
		}

		pensioner.reset_me();
	}

	MPI_Finalize();
}
