#include "Pensioner.hpp"

public Pensioner::Pensioner(int size, int id) {
    this->status = STATUS_NEUTRAL;
    this->size = size;
    this->my_leader_id = this->my_id = id;
    this->is_club_selected = false;
    this->club_array = new bool[CLUB_SIZE] ();
    this->pensioners_status_list = new int[PENSIONERS_NR] ();
    
    this->lamport_time = new Lamport();
    this->ready_cond_var = false;
}

void Pensioner::main_loop() {
	std::thread thread_1(listen, this);
	std::thread thread_2(handle_stuff, this);
	thread_1.join();
	thread_2.join();
}