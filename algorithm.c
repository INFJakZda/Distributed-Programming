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
int noMembers; // N - NUMBER OF MEMBERS
int noClubs;   // K - HOW MANY CLUBS
int entryCost; // M - ENTRY AMOUNT
int memberMoney;
int groupMoney;
int memberId;
int preferedClubId;
int localClock;

/* ASK TAB VALUES */
int *askTab;
#define READY_ASK_TAB 0
#define ACCEPT_ASK_TAB 1
#define REJECT_ASK_TAB 2

/* MY STATUS VALUES */
int myStatus;
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
#define DISAGREE_TO_ENTER_CLUB_MSG 6
#define EXIT_CLUB_MSG 7

//TYPE OF PACKAGE SENDING BETWEEN MEMBERS
MPI_Datatype mpiMsgType;

typedef struct msg_s
{
    int localClock;
    int message;
    int memberId;
    int clubId;
    int memberMoney;
} msg;

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

void *threadFunc()
{
    //msg sendMsg;
    msg recvMsg;
    while (true)
    {
        MPI_Recv(&recvMsg, 1, mpiMsgType, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        localClock = max(recvMsg.localClock, localClock) + 1;
        printf("[myId: %d][from: %d][clock: %d]   Odebrałem wiadomość\n", memberId, recvMsg.memberId, localClock);
    }
}

void createThread()
{
    pthread_t pthreadFunc;
    if (pthread_create(&pthreadFunc, NULL, threadFunc, NULL))
    {
        printf("[myId: %d]Error creating thread\n", memberId);
        free(askTab);
        MPI_Type_free(&mpiMsgType);
        MPI_Finalize();
        exit(-1);
    }
}

void initMember()
{
    memberMoney = (rand() % (entryCost - 2)) + 1;
    groupMoney = memberMoney;
    myStatus = ALONE_STATUS;
    preferedClubId = rand() % noClubs;
    askTab = calloc(noMembers, sizeof(int));

    //RESET askTab to 0 [READY_ASK_TAB]
    for (int i = 0; i < noMembers; i++)
    {
        *(askTab + i) = READY_ASK_TAB;
        if (i == memberId)
            *(askTab + i) = ACCEPT_ASK_TAB;
    }
}

bool isNotEmptyTab()
{
    for (int i = 0; i < noMembers; i++)
    {
        if (*(askTab + i) == 0)
        {
            return true;
        }
    }
    return false;
}

int getRandomMemberID()
{
    int val = rand() % noMembers;

    if (askTab[val] == READY_ASK_TAB)
    {
        if (val != memberId)
            return val;
    }

    for (int i = 0; i < noMembers; i++)
    {
        if (askTab[i] == READY_ASK_TAB)
        {
            if (i != memberId)
                return i;
        }
    }

    return -1;
}

msg createPackage(int localClock, int message, int memberId, int clubId, int memberMoney)
{
    msg package;

    package.localClock = localClock;
    package.message = message;
    package.memberId = memberId;
    package.clubId = clubId;
    package.memberMoney = memberMoney;
    return package;
}

void mainLoop()
{
    //START COMMUNICATE AFTER RANDOM SHORT TIME
    localClock = rand() % noMembers;
    sleep(localClock);

    msg sendMsg;

    while (true)
    {
        initMember();
        
        while (isNotEmptyTab())
        {
            //INCREASE LAMPORT CLOCK
            localClock++;

            //SELECT MEMBER TO ASK FROM askTab
            int selectedMember = getRandomMemberID();
            if (selectedMember == -1)
            {
                printf("ERROR: Selected member == -1");
                break;
            }

            //SEND MESSAGE TO SELECTED MEMBER & CHANGE STATUS
            sendMsg = createPackage(localClock, ASK_TO_JOIN_MSG, memberId, preferedClubId, memberMoney);
            
            MPI_Send(&sendMsg, 1, mpiMsgType, selectedMember, MSG_TAG, MPI_COMM_WORLD);
            myStatus = WAIT_FOR_RESPONSE_STATUS
            printf("[myId: %d][to:   %d][clock: %d]   Zapytanie o dolaczenie do grupy\n", memberId, selectedMember, localClock);

            //WAIT FOR STATUS UPDATE - RECIEVE RESPONSE MSG
            while (myStatus == WAIT_FOR_RESPONSE_STATUS)
            {

            }

            switch (myStatus)
            {
                case LEADER_STATUS:
                    
            }
        }
        //EXIT CLUBS IN DIFFERERNT TIME
        int sleepTime = rand() % noMembers;
        localClock += sleepTime;
        sleep(sleepTime);
        printf("[myId: %d]         [clock: %d]   Spałem %d sekund\n", memberId, localClock, sleepTime);
    }
}

int main(int argc, char *argv[])
{
    /* READ K & M FROM argv */
    noClubs = atoi(argv[1]);
    entryCost = atoi(argv[2]);
    //printf("K = %d M = %d\n", noClubs, entryCost);

    /* INIT MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noMembers);
    MPI_Comm_rank(MPI_COMM_WORLD, &memberId);

    /* PREPARE STRUCT FOR SENDING MESSAGES */
    int blocklengths[MESSAGE_COUNT] = {1, 1, 1, 1, 1};
    MPI_Datatype types[MESSAGE_COUNT] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint offsets[MESSAGE_COUNT];
    offsets[0] = offsetof(msg, localClock);
    offsets[1] = offsetof(msg, message);
    offsets[2] = offsetof(msg, memberId);
    offsets[3] = offsetof(msg, clubId);
    offsets[4] = offsetof(msg, memberMoney);

    MPI_Type_create_struct(MESSAGE_COUNT, blocklengths, offsets, types, &mpiMsgType);
    MPI_Type_commit(&mpiMsgType);
    
    srand(time(0) + memberId);

    //RECIEVING THREAD
    createThread();

    //MAIN SENDING THREAD
    mainLoop();

    printf("[myId: %d] koniec\n", memberId);
    /* MPI FINALIZE */
    MPI_Type_free(&mpiMsgType);
    MPI_Finalize();

    return 0;
}
