#ifndef MAIN_H
#define MAIN_H

#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

//BOOL
typedef int bool;
enum
{
    false,
    true
};

/* CONST VALUES */
#define MESSAGE_COUNT 5
#define MSG_TAG 100

/* GLOBAL VARIABLES (ONLY FOR THREADS) */
extern int noMembers; // N - NUMBER OF MEMBERS
extern int noClubs;   // K - HOW MANY CLUBS
extern int entryCost; // M - ENTRY AMOUNT
extern int memberMoney;
extern int groupMoney;
extern int memberId;
extern int preferedClubId;
extern int localClock;
extern int approveCount;
extern bool threadMoneyFlag;

/* ASK TAB VALUES */
extern int *askTab;
#define READY_ASK_TAB 0
#define ACCEPT_ASK_TAB 1
#define REJECT_ASK_TAB 2

/* MY STATUS VALUES */
extern int myStatus;
#define ALONE_STATUS 0
#define LEADER_STATUS 1
#define MEMBER_STATUS 2
#define ACCEPT_INVITATION_STATUS 3
#define REJECT_INVITATION_STATUS 4
#define ENOUGH_MONEY_STATUS 5
#define ENTER_CLUB_STATUS 6
#define EXIT_CLUB_STATUS 7
#define GROUP_BREAK_STATUS 8

/* MESSAGE TYPE VALUES */
#define ASK_TO_JOIN_MSG 0
#define CONFIRM_JOIN_MSG 1
#define REJECT_JOIN_MSG 2
#define GROUP_BREAK_MSG 3
#define ASK_TO_ENTER_CLUB_MSG 4
#define AGREE_TO_ENTER_CLUB_MSG 5
#define EXIT_CLUB_MSG 6

//TYPE OF PACKAGE SENDING BETWEEN MEMBERS
extern MPI_Datatype mpiMsgType;

typedef struct msg_s
{
    int localClock;
    int message;
    int memberId;
    int preferedClubId;
    int memberMoney;
} msg;

// Declaration of thread condition variable
extern pthread_cond_t cond1;
// declaring mutex
extern pthread_mutex_t lock;

/* FUNCTIONS */
int max(int, int);
msg createPackage(int, int, int, int, int);
void *threadFunc();
void initMember();
bool isNotEmptyTab();
int getRandomMemberID();
void mainLoop();
void leader_func(msg);

#endif
