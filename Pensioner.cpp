#include "Pensioner.hpp"

public Pensioner::Pensioner(int size, int id) {
    this->club_array = new bool[CLUB_SIZE] ();
    this->lamport_time = new Lamport();
    this->status = STATUS_NEUTRAL;
    this->pensioners_status_list = new int[PENSIONERS_NR] ();
    this->is_club_selected = false;
    this->my_leader_id = this->my_id = id;
    this->ready_cond_var = false;
}

void Pensioner::main_loop() {
	std::thread thread_1(listen, this);
	std::thread thread_2(handle_stuff, this);
	thread_1.join();
	thread_2.join();
}

int Pensioner::code_func_control(int code, int source_id) {
    if(source_id == this->my_id) {
    	return 0;
    }
    switch(code) {
        case MSG_CODE_ASK_TO_JOIN:
        	if((this->status == STATUS_NEUTRAL) || (this->status == STATUS_ASK)) {
        		// Process
        	}
        	else {
        		// REJECT
        	}
            break;

        case MSG_CODE_CONFIRM_JOIN:
        	this->status = STATUS_IN_GROUP;
            break;

        case MSG_CODE_REJECT_JOIN:
        	this->pensioners_status_list[source_id] = PENSIONERS_STATUS_LIST_REJECTED;
            break;

        case MSG_CODE_RMOVE_GROUP:
        	this->status = STATUS_REMOVE_GROUP;
            break;

        case MSG_CODE_CHOOSE_CLUB:
           this->status = STATUS_CHOOSE_CLUB;
            break;

        case MSG_CODE_EXIT_CLUB:
        	this->status = STATUS_FINISH;
            break;

        case MSG_CODE_CONFIRM_RECV_MSG:
            break;

        case MSG_CODE_SET_LEADER:
            if(my_id < source_id) {
            	this->my_leader_id = source_id;
            }
            else {
            	this->my_leader_id = my_id;
            } 
            break;

        default:
            std::cout << "#_" << this->my_id << " : MSG_CODE_UNDEFINE_CODE" << std::endl;
            break;
    }
    //MPI_Send(,,MPI_INT, status.MPI_SOURCE, DEFAULT_TAG_VAL, MPI_COMM_WORLD);
    return 1;
}

void Pensioner::listen() {
    int code;
    MPI_Status status;
    bool loop_control = false;

    while(!loop_control) {
        MPI_Recv(&code, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //source id in status.MPI_SOURCE
        if(code_func_control(code, status.MPI_SOURCE) == 0) {
        	loop_control = true;
        }
    }
}

void Pensioner::handle_stuff() {
	bool loop_control = false;
	while(!loop_control) {
		if(do_stuff() != 1) {
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