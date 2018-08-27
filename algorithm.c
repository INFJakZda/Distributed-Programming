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

/* ASK TAB VALUES */
#define READY_ASK_TAB 0
#define ACCEPT_ASK_TAB 1
#define REJECT_ASK_TAB 2

/* CONST VALUES */
#define MESSAGE_COUNT 5
#define MSG_TAG 100

/* MY STATUS VALUES */
#define ALONE_STATUS 0
#define WAIT_FOR_RESPONSE_STATUS -1
#define LEADER_STATUS 1
#define MEMBER_STATUS 2
#define ACCEPT_INVITATION_STATUS 3
#define REJECT_INVITATION_STATUS 4
#define ENOUGH_MONEY_STATUS 5
#define ENTER_CLUB_STATUS 6
#define EXIT_CLUB_STATUS 7
#define GROUP_BREAK_STATUS 8
#define REBOOT_STATUS 9

/* GLOBAL VARIABLES (ONLY FOR THREADS) */
int noMembers; // N - NUMBER OF MEMBERS
int noClubs;   // K - HOW MANY CLUBS
int entryCost; // M - ENTRY AMOUNT
int memberMoney;
int groupMoney;
int memberId;
int preferedClubId;
int myStatus;
int localClock;
int *askTab;
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
        printf("[myId: %d][%d][%d]   Odebrałem wiadomość od %d\n", memberId, recvMsg.localClock, localClock, recvMsg.memberId);
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
    memberMoney = ((rand() % (entryCost - 2)) + 1);
    groupMoney = memberMoney;
    myStatus = ALONE_STATUS;
    preferedClubId = rand() % noClubs + 1;
    askTab = calloc(noMembers, sizeof(int));

    //Reset askTab
    for (int i = 0; i < noMembers; i++)
    {
        *(askTab + i) = READY_ASK_TAB;
        if (i == memberId)
            *(askTab + i) = ACCEPT_ASK_TAB;
    }
}

bool isEmptyTab()
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
    localClock = rand() % (noMembers - 1);
    sleep(localClock);
    while (true)
    {
        initMember();
        msg sendMsg;
        while (isEmptyTab())
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

            //SEND MESSAGE TO SELECTED MEMBER
            sendMsg = createPackage(localClock, 0, memberId, preferedClubId, memberMoney);
            MPI_Send(&sendMsg, 1, mpiMsgType, selectedMember, MSG_TAG, MPI_COMM_WORLD);
            printf("[myId: %d][%d]   Zapytanie o dolaczenie do grupy do memberId = %d\n", memberId, localClock, selectedMember);
            askTab[selectedMember] = 1;
            break;
        }
        sleep(rand() % noMembers - 1);
    }
}

int main(int argc, char *argv[])
{
    /* READ K & M FROM argv */
    noClubs = atoi(argv[1]);
    entryCost = atoi(argv[2]);
    printf("K = %d M = %d\n", noClubs, entryCost);

    /* INIT MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noMembers);
    MPI_Comm_rank(MPI_COMM_WORLD, &memberId);
    printf("[myId: %d] size = %d\n", memberId, noMembers);

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
    printf("[myId: %d] dziala\n", memberId);
    srand(time(0) + memberId);
    //RECIEVING THREAD
    createThread();

    //MAIN SENDING THREAD
    mainLoop();

    printf("[myId: %d] koniec\n", memberId);
    sleep(12);
    /* MPI FINALIZE */
    MPI_Type_free(&mpiMsgType);
    MPI_Finalize();

    return 0;
}
