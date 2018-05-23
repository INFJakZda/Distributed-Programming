#include "Pensioner.hpp"

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

void Pensioner::listen(Pensioner *p) {
    int code;
    MPI_Status status;
    bool loop_control = false;

    while(!loop_control) {
        MPI_Recv(&code, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //source id in status.MPI_SOURCE
        if(p->code_func_control(code, status.MPI_SOURCE) == 0) {
        	loop_control = true;
        }
    }
}