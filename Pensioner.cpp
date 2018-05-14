#include "Pensioner.h"

public Pensioner::Pensioner(int id) {
    this->club_array = new bool[CLUB_SIZE] ();
    this->is_leader = true;
    this->lamport_time = new Lamport();
    this->status = STATUS_NEUTRAL;
    this->pensioners_status_list = new int[PENSIONERS_NR] ();
    this->is_club_selected = false;
    this->my_id = id;
    this->thread_kill = false;
}

void Pensioner::grant_money() {

}

void Pensioner::code_func_control(int code, int source_id) {
    switch(code) {
        case MSG_CODE_ASK_TO_JOIN:
            break;
        case MSG_CODE_CONFIRM_JOIN:
            break;
        case MSG_CODE_REJECT_JOIN:
            break;
        case MSG_CODE_RMOVE_GROUP:
            break;
        case MSG_CODE_CHOOSE_CLUB:
            break;
        case MSG_CODE_ASK_ABOUT_CLUB:
            break;
        case MSG_CODE_EXIT_CLUB:
            break;
        case MSG_CODE_CONFIRM_RECV_MSG:
            break;
        case MSG_CODE_ASK_TO_JOIN:
            break;
        case MSG_CODE_SET_LEADER:
            //recive leader id
            //this->my_leader_id = 
            break;
        default:
            //send MSG_CODE_UNDEFINE_CODE;
            break;
    }
    //MPI_Send(,,MPI_INT, status.MPI_SOURCE, DEFAULT_TAG_VAL, MPI_COMM_WORLD);
}

void Pensioner::signal_callback_handler(int signum)
{
    std::cout << "#DEBUG: Signum = " << signum << std""endl;
    thread->thread_kill = true;    
}

void Pensioner::listen() {
    int code;
    MPI_Status status;

    while(!(this->thread_kill)) {
        MPI_Recv(&code, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //source id in status.MPI_SOURCE
        code_func_control(code, status.MPI_SOURCE);
    }
}

void Pensioner::asking() {

}

void Pensioner::proc_leader() {

}

void Pensioner::no_proc_leader() {
    
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

void Pensioner::choose_club() {
    int index = rand()%(CLUB_SIZE);
    while(!(this->club_array[index])) {
        index = rand()%(CLUB_SIZE);
    }

    this->club_array[index] = true;
    MPI_Send();
}

void Pensioner::lamport_time_stamp_tick() {
    this->lamport_time.time_stamp_tick();
}

unsigned long long get_lamport_time_stamp() {
    return this->lamport_time.get_time_stamp();
}

unsigned int Pensioner::get_money_amount() { 
    return this->money_amount;
}

void Pensioner::set_money_amount() {
    this->money_amount = (rand()%(ENTRY_AMOUNT - 1) + 1);
}

bool Pensioner::check_if_leader() {
    return this->is_leader;
}

void Pensioner::set_as_leader(bool val) {
    this->is_leader = val;
}

void Pensioner::set_group_money(unsigned int money) { 
    this->group_money = money;
}

unsigned int Pensioner::get_group_money() { 
    return this->group_money;
}

void set_status(unsigned int stat) {
    this->status = stat;
}
unsigned int get_status() {
    return this->status;
}