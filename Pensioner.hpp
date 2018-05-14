#ifndef PENSIONER_H
#define PENSIONER_H

#include "main.hpp"
#include "Lamport.hpp"

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
}
#endif
