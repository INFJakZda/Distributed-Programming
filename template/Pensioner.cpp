#include "Pensioner.h"

public Pensioner::Pensioner() {
        this->club_array = new bool[CLUB_SIZE];
}

void Pensioner::grant_money() {

}

void Pensioner::listen() {

}

void Pensioner::asking() {

}

bool* Pensioner::get_club_array() { return this->club_array; }

unsigned int Pensioner::get_money_amount() { return this->money_amount; }

bool Pensioner::check_if_leader() { return this->is_leader; }

void Pensioner::set_group_money(unsigned int money) { this->group_money = money; }

unsigned int Pensioner::get_group_money() { return this->group_money; }