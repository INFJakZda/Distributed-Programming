#ifndef PENSIONER_H
#define PENSIONER_H

#include "main.hpp"
#include "Lamport.hpp"

#define PENSIONERS_NR 20
#define CLUB_SIZE 50
#define ENTRY_AMOUNT 100

#define 

class Pensioner {
    private: 
        unsigned int money_amount;
        bool is_leader;
        unsigned int group_money;
        Lamport lamport_time;
        unsigned int status;
        
    public:
        bool* club_array;
        int* pensioners_status_list;

    public: 
        Pensioner();

        void grant_money();
        void listen();
        void asking();
        void proc_leader();
        void no_proc_leader();
        void reset_me();
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
}
#endif
