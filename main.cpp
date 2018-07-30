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
#define PROC_STATUS_ENTER_CLUB 6
#define PROC_STATUS_EXIT_CLUB 7
#define PROC_STATUS_GROUP_BREAK 8
#define PROC_STATUS_REBOOT 9

/* MESSAGE STATUS */
#define MESSAGE_STATUS_ASK_TO_JOIN 0
#define MESSAGE_STATUS_CONFIRM_JOIN 1
#define MESSAGE_STATUS_REJECT_JOIN 2
#define MESSAGE_STATUS_DISSOLUTION_GROUP 3
#define MESSAGE_STATUS_ASK_TO_ENTER 4
#define MESSAGE_STATUS_AGREE_TO_ENTER_CLUB 5
#define MESSAGE_STATUS_DISAGREE_TO_ENTER_CLUB 6
#define MESSAGE_STATUS_EXIT_CLUB 7

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
unsigned int preferClubId;
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

#define MSG_TAG 100

void *threadFunc();

unsigned int checkIfSomeoneToAsk()
{
	for(int i=0; i<N; i++)
	{
		if(Arry_Of_Members[i] == ARRAY_VAL_NOT_ASKED)
			return TRUE;
	}
	return FALSE;
}

int getRandomMemberID()
{
	int val = rand()%N;
	
	if(Arry_Of_Members[val] == ARRAY_VAL_NOT_ASKED)
	{
		if(i != MEMBER_ID)
			return val;
	}

	for(int i=0; i<N; i++)
	{
		if(Arry_Of_Members[i] == ARRAY_VAL_NOT_ASKED)
		{
			if(i != MEMBER_ID)
				return i;
		}
	}
			
	return -1;
}

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
	
	localClock = 0;
	
	pthread_t  pthreadFunc;
	if(pthread_create(&pthreadFunc, NULL, threadFunc, NULL)) {
		fprintf(stderr, "[ID: %d]Error creating thread\n", MEMBER_ID);
		free(Arry_Of_Members);
		MPI_Type_free(&mpi_msg_type);
		MPI_Finalize();
		return 1;
	}
	
	while(TRUE)
	{
		/* INFINITE LOOP */
		amount = rand()%(M - 2) + 1;	//GET RANDOM AMOUNT FROM RANGE <0; M-1>
		//Choose club
		if(K > 1)
		{
			preferClubId = rand()%K;			// GET RANDOM PREFERED CLUB ID
		}
		else
		{
			preferClubId = 0;	
		}
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
		
		short second_loop_control = TRUE;
		while(checkIfSomeoneToAsk() && second_loop_control)
		{
			++localClock;
			msg_to_send.clock = localClock;
			msg_to_send.message = MESSAGE_STATUS_ASK_TO_JOIN;
			msg_to_send.memberId = MEMBER_ID;
			msg_to_send.clubId = preferClubId;
			msg_to_send.amount = amount;
			
			int tempVal = getRandomMemberID();
			if(tempVal == -1)
			{
				printf("[ID: %d][TIME: %d]: No one to ask!", MEMBER_ID, localClock);
				second_loop_control = FALSE;
			}
			else
			{
				MPI_Send(&msg_to_send, 1, mpi_msg_type, tempVal, MSG_TAG, MPI_COMM_WORLD);
				printf("[ID: %d][TIME: %d]: Ask member to join [ID: %d]", MEMBER_ID, localClock, tempVal);
								
				
				while((myStatus==PROC_STATUS_ALONE) || (myStatus == PROC_STATUS_MEMBER) || (myStatus == PROC_STATUS_LEADER)) 
				{
					/* WAIT FOR UPDATE MYSTATUS */	
				}
				
				if(myStatus == PROC_STATUS_ACCEPT_INVITATION)
				{
					myStatus = PROC_STATUS_LEADER;
					if(groupAmount >= M)
					{
						pritnf("[ID: %d][Time: %d]: Enough money!", MEMBER_ID, localClock);
						second_loop_control=FALSE;						
					}
				}
				
				if(myStatus == PROC_STATUS_REJECT_INVITATION)
				{
					myStatus = PROC_STATUS_ALONE;
				}
				
				if(myStatus == PROC_STATUS_GROUP_BREAK)
				{
					groupAmount = money;
					myStatus = PROC_STATUS_ALONE;
					pritnf("[ID: %d][Time: %d]: Group break(#1)!", MEMBER_ID, localClock)
				}
				
				if(status == PROC_STATUS_EXIT_CLUB)
				{
					second_loop_control = FALSE;
					myStatus = PROC_STATUS_ALONE;
					pritnf("[ID: %d][Time: %d]: Go out from club!", MEMBER_ID, localClock);
				}
			}
		}

		if((groupAmount < M) && (myStatus==PROC_STATUS_LEADER))
		{
			pritnf("[ID: %d][Time: %d]: Group break(#2)!", MEMBER_ID, localClock)
			groupAmount = money;
			
			for(int i=0; i<N; i++)
			{
				if(Arry_Of_Members[i]==ARRAY_VAL_ASKED_ACCEPT)
				{
					++localClock;
					msg_to_send.clock = localClock;
					msg_to_send.message = MESSAGE_STATUS_DISSOLUTION_GROUP;
					msg_to_send.memberId = MEMBER_ID;
					msg_to_send.clubId = preferClubId;
					msg_to_send.amount = amount;
					MPI_Send(&msg_to_send, 1, mpi_msg_type, i, MSG_TAG, MPI_COMM_WORLD);
				}
			}
		}
		
		if((groupAmount >= M) && (myStatus==PROC_STATUS_LEADER))
		{
			pritnf("[ID: %d][Time: %d]: Try to go to club!", MEMBER_ID, localClock)
			myStatus = PROC_STATUS_ENOUGH_MONEY;
			pritnf("[ID: %d][Time: %d]: Choose club number %d", MEMBER_ID, localClock, preferClubId);
			
			for(int i=0; i<N; i++)
			{
				if(i!=MEMBER_ID)
				{
					++localClock;
					msg_to_send.clock = localClock;
					msg_to_send.message = MESSAGE_STATUS_ASK_TO_ENTER;
					msg_to_send.memberId = MEMBER_ID;
					msg_to_send.clubId = preferClubId;
					msg_to_send.amount = amount;
					MPI_Send(&msg_to_send, 1, mpi_msg_type, i, MSG_TAG, MPI_COMM_WORLD);
					pritnf("[ID: %d][Time: %d]: Choose club number %d, Ask MEMBER %d for permission", MEMBER_ID, localClock, preferClubId, i);
				}
			}
		}

		/* ************************************************************* */
	}
	
	if(pthread_join(pthreadFunc, NULL)) {
		fprintf(stderr, "[ID: %d]Error joining thread\n", MEMBER_ID);
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
	MPI_Status mpi_status;
	msg recive;
	
}
