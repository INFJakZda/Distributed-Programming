#include <mpi.h>
#include <stdio.h>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>

#define PENSIONERS_NR 20
#define CLUB_SIZE 50
#define ENTRY_AMOUNT 100

#define STATUS_NEUTRAL 0
#define STATUS_IN_GROUP 1
#define STATUS_REMOVE_GROUP 2
#define STATUS_GO_TO_CLUB 3
#define STATUS_FINISH 4

#define PENSIONERS_STATUS_LIST_NO_ASKED 0
#define PENSIONERS_STATUS_LIST_IN_MY_GROUP 1
#define PENSIONERS_STATUS_LIST_REJECTED 2

#define MSG_CODE_ASK_TO_JOIN 0
#define MSG_CODE_CONFIRM_JOIN 1
#define MSG_CODE_REJECT_JOIN 2
#define MSG_CODE_RMOVE_GROUP 3
#define MSG_CODE_CHOOSE_CLUB 4
#define MSG_CODE_ASK_ABOUT_CLUB 5
#define MSG_CODE_EXIT_CLUB 6
#define MSG_CODE_CONFIRM_RECV_MSG 7
#define MSG_CODE_UNDEFINE_CODE 66
#define MSG_CODE_SET_LEADER 666

#define DEFAULT_TAG_VAL 100

class Pensioner {
    private: 
        unsigned int money_amount;
        unsigned int status;
        unsigned int group_money;
        unsigned int my_leader_id;
        unsigned int my_id;
        bool is_leader;
        bool is_club_selected;
        bool* club_array;
        int* pensioners_status_list;
        int selected_club_id;
        Lamport lamport_time;
        std::vector <unsigned int> group_members_id;
		bool thread_kill;

    public:
        Pensioner();

        void grant_money();
        void listen();
        void asking();
		void proc_leader();
		void no_proc_leader();
        void reset_me(int);
        void go_to_club();

        void lamport_time_stamp_tick();
        unsigned long long get_lamport_time_stamp();
        
        unsigned int get_money_amount();
        void set_money_amount();
        
        bool check_if_leader();
        void set_as_leader(bool);
        
        void set_group_money(unsigned int);
        unsigned int get_group_money();
        
        void set_status(unsigned int);
        unsigned int get_status();

    private:
        void code_func_control(int, int);
		void signal_callback_handler(int);
}

class Lamport {
private:
	unsigned long long time_stamp;
	
public:
	Lamport() { 
        this->time_stamp = 0;
    }
	
	unsigned long long get_time_stamp() {
		return this->time_stamp;
	}

	void time_stamp_tick() { this->times_tamp++; }
};

/*  *************************** MAIN ************************************************ */
int main(int argc, char **argv)
{
	srand(time(NULL));

	int size, rank;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Pensioner pensioner = new Pensioner(rank);

    std::thread pensioner_thread(&Pensioner::listen, &pensioner);	

	while(true){
		pensioner.set_money_amount();  
		pensioner.lamport_time_stamp_tick();

		bool loop_control = true;
		while(loop_control){
			if(pensioner.check_if_leader()) {
				if(pensioner.get_group_money() > ENTRY_AMOUNT) {
					pensioner.choose_club();
				}
				else {
					pensioner.proc_leader();
				}
			}
			else {
				pensioner.no_proc_leader();
			}

			if(pensioner.get_status() == STATUS_REMOVE_GROUP) {
				pensioner.lamport_time_stamp_tick();
			}

			if(pensioner.get_status() == STATUS_GO_TO_CLUB) {
				pensioner.lamport_time_stamp_tick();
				pensioner.go_to_club();
			}

			if(pensioner.get_status() == STATUS_FINISH) {
				pensioner.lamport_time_stamp_tick();
				loop_control = false;
			}		
			
		}
		pensioner.reset_me(--STATUS_REMOVE_GROUP);
	}

	pensioner_thread.join();

	MPI_Finalize();
}

/* *************************************************************** */


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