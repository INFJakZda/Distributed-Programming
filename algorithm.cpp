#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* ASK TAB VALUES */
#define READY_TO_ASK 0
#define ASK_ACCEPT 1
#define ASK_REJECT 2

/* CONST VALUES */
#define MESSAGE_COUNT 5
#define MSG_TAG 100

/* GLOBAL VARIABLES (ONLY FOR THREADS) */
int noMembers;      // N - NUMBER OF MEMBERS
int entryAmount;    // M - ENTRY AMOUNT
int noClubs;        // K - HOW MANY CLUBS
int memberMoney;     
int groupMoney;     
unsigned int *askTab;
int memberId;
unsigned int localClock;

MPI_Datatype mpiMsgType;

typedef struct msg_s {
    unsigned int clock;
    short message;
    unsigned int memberId;
    unsigned int clubId;
    unsigned int money;
} msg;

int main (int argc, char *argv[])
{
    srand( time(NULL) );

    /* READ K & M FROM argv */
    noClubs = atoi( argv[1] );
    entryAmount = atoi( argv[2] );
    printf("K = %d M = %d\n", no)

    MPI_Init(NULL, NULL);

    MPI_Comm_size( MPI_COMM_WORLD, &noMembers );
    MPI_Comm_rank( MPI_COMM_WORLD, &memberId );

    /* PREPARE STRUCT FOR SENDING MESSAGES */
    int blocklengths[MESSAGE_COUNT] = {1, 1, 1, 1, 1};
    MPI_Datatype types[MESSAGE_COUNT] = {MPI_UNSIGNED, MPI_SHORT, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};

    MPI_Aint offsets[MESSAGE_COUNT];
    offsets[0] = offsetof(msg, clock);
    offsets[1] = offsetof(msg, message);
    offsets[2] = offsetof(msg, memberId);
    offsets[3] = offsetof(msg, clubId);
    offsets[4] = offsetof(msg, amount);

    MPI_Type_create_struct(MESSAGE_COUNT, blocklengths, offsets, types, &mpiMsgType);
    MPI_Type_commit(&mpiMsgType);

    askTab = malloc(sizeof(int) * N);
    msg msgToSend;

    localClock = 0;

    MPI_Finalize();

    return 0;
}