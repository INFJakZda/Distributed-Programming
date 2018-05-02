#include "Pensioner.h"

public Pensioner::Pensioner() {
    this->club_array = new bool[CLUB_SIZE] ();
    this->is_leader = true;
}

void Pensioner::grant_money() {

}

void Pensioner::listen() {

}

void Pensioner::asking() {

}

void Pensioner::thread_communication() {
    std::thread t1(this->listen);
    std::thread t2(this->asking);

    t1.join();
    t2.join();
}

void Pensioner::reset_me() {
    this->money_amount = 0;
    this->group_money = 0;
    this->is_leader = true;
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void Pensioner::choose_club() {
    int index = rand()%(CLUB_SIZE);
    while(!(this->club_array[index])) {
        index = rand()%(CLUB_SIZE);
    }

    this->club_array[index] = true;
    MPI_Send();
}

bool* Pensioner::get_club_array() { 
    return this->club_array; 
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