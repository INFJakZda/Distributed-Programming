#include "main.h"

/* GLOBAL VARIABLES (ONLY FOR THREADS) */
int noMembers = 0; // N - NUMBER OF MEMBERS
int noClubs = 0;   // K - HOW MANY CLUBS
int entryCost = 0; // M - ENTRY AMOUNT
int memberMoney = 0;
int groupMoney = 0;
int memberId = 0;
int preferedClubId = 0;
int localClock = 0;
int approveCount = 0;
bool threadMoneyFlag = false;
/* ASK TAB VALUES */
int *askTab;
/* MY STATUS VALUES */
int myStatus = 0;
//TYPE OF PACKAGE SENDING BETWEEN MEMBERS
MPI_Datatype mpiMsgType;

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
    offsets[3] = offsetof(msg, preferedClubId);
    offsets[4] = offsetof(msg, memberMoney);

    MPI_Type_create_struct(MESSAGE_COUNT, blocklengths, offsets, types, &mpiMsgType);
    MPI_Type_commit(&mpiMsgType);

    srand(time(0) + memberId);

    //INIT VALUE - WHEN RECIEVE MSG BEFORE MAINLOOP START
    if (memberMoney == 0)
    {
        threadMoneyFlag = true;
        memberMoney = (rand() % (entryCost - 2)) + 1;
        printf("[myId: %d][clock: %d]           my memberMoney = %d\n", memberId, localClock, memberMoney);
    }

    //RECIEVING THREAD
    pthread_t pthreadFunc;
    if (pthread_create(&pthreadFunc, NULL, threadFunc, NULL))
    {
        printf("[myId: %d]Error creating thread {%d}\n", memberId, myStatus);
        free(askTab);
        MPI_Type_free(&mpiMsgType);
        MPI_Finalize();
        exit(-1);
    }

    //MAIN SENDING THREAD
    mainLoop();

    printf("[myId: %d] koniec\n", memberId);
    /* MPI FINALIZE */
    MPI_Type_free(&mpiMsgType);
    MPI_Finalize();

    return 0;
}
