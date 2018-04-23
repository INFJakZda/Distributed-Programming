#ifndef PENSIONER_H
#define PENSIONER_H

#include "main.hpp"

class Pensioner {

private: 
    bool* club_array;
    unsigned int money_amount;
    bool is_leader;
    unsigned int group_money;

public: 
    Pensioner();

    void grant_money();
    void listen();
    void asking();

    bool* get_club_array();
    unsigned int get_money_amount();
    bool check_if_leader();
    void set_group_money();
    unsigned int get_group_money(unsigned int);
}
#endif
