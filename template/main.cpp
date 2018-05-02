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
		pensioner.lamport_time_stamp_tick();

		bool loop_control = true;
		while(loop_control){
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
				pensioner.no_proc_leader();
			}

			if(pensioner.get_status() == ) {
				loop_control = false;
			}
			
			pensioner.lamport_time_stamp_tick();
		}
		pensioner.reset_me();
		pensioner.lamport_time_stamp_tick();
	}

	MPI_Finalize();
}
