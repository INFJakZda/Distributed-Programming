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

void Pensioner::reset_me() {
        this->money_amount = 0;
        this->group_money = 0;
        this->is_leader = true;
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

bool* Pensioner::get_club_array() { 
        return this->club_array; 
}

unsigned int Pensioner::get_money_amount() { 
        return this->money_amount;
}

void Pensioner::set_money_amount() {
        srand(time(NULL));
        this->money_amount = (rand()%(ENTRY_AMOUNT - 1) + 1);
}

bool Pensioner::check_if_leader() {
        return this->is_leader;
}

void Pensioner::set_group_money(unsigned int money) { 
        this->group_money = money;
}

unsigned int Pensioner::get_group_money() { 
        return this->group_money;
}