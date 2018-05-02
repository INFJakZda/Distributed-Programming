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
        void thread_communication();
        void reset_me();
        void go_to_club();

        bool* get_club_array();
        unsigned int get_money_amount();
        void set_money_amount();
        bool check_if_leader();
        void set_as_leader(bool);
        void set_group_money(unsigned int);
        unsigned int get_group_money();
}
#endif
