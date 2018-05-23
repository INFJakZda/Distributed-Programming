#include "Pensioner.hpp"

void Pensioner::handle_stuff() {
	bool loop_control = false;
	while(!loop_control) {
		if(p->do_stuff() != 1) {
			loop_control = true;
		}
	}
}

int Pensioner::do_stuff() {
	/*
	pensioner.set_money_amount();  
    pensioner.lamport_time_stamp_tick();
    bool loop_control = true;
    while(loop_control){
        
        if(pensioner.check_if_leader()) {
            if(pensioner.get_group_money() >= ENTRY_AMOUNT) {
                pensioner.choose_club();
            }
            else {
                pensioner.proc_leader();
            }
        }

        if(pensioner.get_status() == STATUS_IN_GROUP) {
            pensioner.lamport_time_stamp_tick();
        } 
        else if(pensioner.get_status() == STATUS_REMOVE_GROUP) {
            pensioner.lamport_time_stamp_tick();
        }
        else if(pensioner.get_status() == STATUS_CHOOSE_CLUB) {
            pensioner.lamport_time_stamp_tick();
        }
        else if(pensioner.get_status() == STATUS_GO_TO_CLUB) {
            pensioner.lamport_time_stamp_tick();
            pensioner.go_to_club();
        }
		else if(pensioner.get_status() == STATUS_FINISH) {
            pensioner.lamport_time_stamp_tick();
            loop_control = false;
        }		
        
    }
    pensioner.reset_me(--STATUS_REMOVE_GROUP);
    MPI_Send(&rank, 1,MPI_INT, rank, DEFAULT_TAG_VAL, MPI_COMM_WORLD);
	*/
	return 1;
}

void Pensioner::reset_me(int arg) {
    if(arg == STATUS_REMOVE_GROUP) {
        this->group_money = 0;
        this->is_leader = true;
        
        for(int i = 0; i < CLUB_SIZE; i++)
            this->club_array[i] = false;
        for(int i = 0; i < PENSIONERS_NR; i++)
            this->pensioners_status_list[i] = PENSIONERS_STATUS_LIST_NO_ASKED;
    }
    else {
        this->money_amount = 0;
        this->group_money = 0;
        this->is_leader = true;
        for(int i = 0; i < CLUB_SIZE; i++)
            this->club_array[i] = false;
        for(int i = 0; i < PENSIONERS_NR; i++)
            this->pensioners_status_list[i] = PENSIONERS_STATUS_LIST_NO_ASKED;
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}