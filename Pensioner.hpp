#ifndef _PENSIONER_HPP
#define _PENSIONER_HPP

#include <mpi.h>
#include <stdio.h>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "Lamport.hpp"

#define STATUS_NEUTRAL 0
#define STATUS_ASK 1
#define STATUS_BE__ASKED 2
#define STATUS_IN_GROUP 3
#define STATUS_REMOVE_GROUP 4
#define STATUS_CHOOSE_CLUB 5
#define STATUS_GO_TO_CLUB 6
#define STATUS_FINISH 7

#define PENSIONERS_STATUS_LIST_NO_ASKED 0
#define PENSIONERS_STATUS_LIST_ASKED 1
#define PENSIONERS_STATUS_LIST_IN_MY_GROUP 2
#define PENSIONERS_STATUS_LIST_REJECTED 3

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
        bool is_club_selected;
        bool* club_array;
        int* pensioners_status_list;
        int selected_club_id;
        Lamport lamport_time;
        std::mutex mutx;
        std::condition_variable cond_var;
        bool ready_cond_var;

    public:
        Pensioner(int, int);
        main_loop();

    private:
        void listen();
        void handle_stuff();
        int do_stuff();
        void reset_me(int);
        int code_func_control(int, int);
}

#endif