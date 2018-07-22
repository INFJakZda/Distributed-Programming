#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <pthread.h>

/* BOOLEAN VALUES */
#define FALSE 0
#define TRUE 1

/* PROCES STATUS */
#define PROC_STATUS_ALONE 0
#define PROC_STATUS_LEADER 1
#define PROC_STATUS_MEMBER 2
#define PROC_STATUS_ACCEPT_INVITATION 3
#define PROC_STATUS_REJECT_INVITATION 4
#define PROC_STATUS_ENOUGH_MONEY 5
#define PROC_STATUS_REBOOT 6

/* MESSAGE STATUS */
#define MESSAGE_STATUS_ASK_TO_JOIN 0
#define MESSAGE_STATUS_CONFIRM_JOIN 1
#define MESSAGE_STATUS_REJECT_JOIN 2
#define MESSAGE_STATUS_DISSOLUTION_GROUP 3
#define MESSAGE_STATUS_ASK_TO_ENTER 4
#define MESSAGE_STATUS_AGREE_TO_ENTER_CLUB 5
#define MESSAGE_STATUS_DISAGREE_TO_ENTER_CLUB 6
#define MESSAGE_STATUS_EXIT_CLUB

/* ARRAY VALUES */
#define ARRAY_VAL_NOT_ASKED 0
#define ARRAY_VAL_ASKED_ACCEPT 1
#define ARRAY_VAL_ASKED_REJECT 2

/* GLOBAL VARIABLES (ONLY FOR THREADS) */
int N; // NUMBER OF MEMBERS
int M; // AMOUNT
int K; // HOW MANY CLUBS
unsigned int *Arry_Of_Members;
int MEMBER_ID; // ID OF MEMBER
MPI_Datatype mpi_msg_type;
unsigned int localClock;
unsigned int clubId;
unsigned int amount;
unsigned int groupAmount;
unsigned int coutApproveFromMembersToEnterToClub;
unsigned int myStatus;

#define MESSAGE_COUNT 5
typedef struct msg_s {
        unsigned int clock;
        short message;
        unsigned int memberId;
        unsigned int clubId;
        unsigned int amount;
}msg;

void *threadFunc();

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	// GET FROM ARGS
	K = atoi(argv[1]);
	M = atoi(argv[2]);
	printf("K = %d M = %d\n", K, M);
	
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &MEMBER_ID);
	MPI_Aint offsets[MESSAGE_COUNT];
	MPI_Datatype types[MESSAGE_COUNT] = {MPI_UNSIGNED, MPI_SHORT, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
	offsets[0] = offsetof(msg, clock);
	offsets[1] = offsetof(msg, message);
	offsets[2] = offsetof(msg, memberId);
	offsets[3] = offsetof(msg, clubId);
	offsets[4] = offsetof(msg, amount);
	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_msg_type);
    MPI_Type_commit(&mpi_msg_type);
	
	Arry_Of_Members = malloc(sizeof(int) * N);
	msg msg_to_send;
	int search_Arry_Of_Members;
	
	localClock = 0;
	
	pthread_t  pthreadFunc;
	if(pthread_create(&pthreadFunc, NULL, threadFunc, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		free(Arry_Of_Members);
		MPI_Type_free(&mpi_msg_type);
		MPI_Finalize();
		return 1;
	}
	
	while(TRUE)
	{
		/* INFINITE LOOP */
		amount = rand()%(M - 2) + 1;	//GET RANDOM AMOUNT FROM RANGE <0; M-1>
		if(K > 1)
			clubId = rand()%K;			// GET RANDOM PREFERED CLUB ID
		else
			clubId = 0;
		
		groupAmount = amount;
		coutApproveFromMembersToEnterToClub = 0;
		myStatus = PROC_STATUS_ALONE;
		for(int i = 0; i < N; i++)
		{
			if(i == MEMBER_ID)
				Arry_Of_Members[i] = ARRAY_VAL_ASKED_ACCEPT;
			else
				Arry_Of_Members[i] = ARRAY_VAL_NOT_ASKED;
		}
		
		search_Arry_Of_Members = 0;
		while(search_Arry_Of_Members == N)
		{
			if(Arry_Of_Members[search_Arry_Of_Members] == ARRAY_VAL_NOT_ASKED)
			{
				
				++search_Arry_Of_Members;
			}
			else
				++search_Arry_Of_Members;
		}
		/* ************************************************************* */
	}
	
	if(pthread_join(pthreadFunc, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		free(Arry_Of_Members);
		MPI_Type_free(&mpi_msg_type);
		MPI_Finalize();
		return 2;
	}
	
	free(Arry_Of_Members);
	MPI_Type_free(&mpi_msg_type);
	MPI_Finalize();
	
	return 0;
}

void *threadFunc()
{
	
}
