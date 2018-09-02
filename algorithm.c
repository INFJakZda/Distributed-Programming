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
int approveCount;
bool threadMoneyFlag = false;

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
    int preferedClubId;
    int memberMoney;
} msg;

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

msg createPackage(int localClock, int message, int memberId, int preferedClubId, int memberMoney)
{
    msg package;

    package.localClock = localClock;
    package.message = message;
    package.memberId = memberId;
    package.preferedClubId = preferedClubId;
    package.memberMoney = memberMoney;
    return package;
}

void *threadFunc()
{
    msg sendMsg;
    msg recvMsg;
    if (memberMoney == 0)
    {
        threadMoneyFlag = true;
        memberMoney = (rand() % (entryCost - 2)) + 1;
        printf("[myId: %d][clock: %d]           my memberMoney = %d\n", memberId, localClock, memberMoney);
    }
    while (true)
    {
        MPI_Recv(&recvMsg, 1, mpiMsgType, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        localClock = max(recvMsg.localClock, localClock) + 1;
        printf("[myId: %d][clock: %d][from: %d] Odebrałem wiadomość {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);

        //2 TODO what's that if else ?!?!?!?
        if (myStatus == ENOUGH_MONEY_STATUS && recvMsg.message == ASK_TO_ENTER_CLUB_MSG)
        {
            if (recvMsg.preferedClubId != preferedClubId)
            {
                localClock++;
                sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, recvMsg.memberId, recvMsg.preferedClubId, myStatus);
            }
            else
            {
                if (recvMsg.localClock < localClock)
                {
                    localClock++;
                    sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                    MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                    printf("[myId: %d][clock: %d][to:   %d] Pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, recvMsg.memberId, recvMsg.preferedClubId, myStatus);
                }
            }
        }

        //3
        if ((myStatus != ALONE_STATUS && myStatus != GROUP_BREAK_STATUS) && recvMsg.message == ASK_TO_JOIN_MSG)
        {
            localClock++;
            sendMsg = createPackage(localClock, REJECT_JOIN_MSG, memberId, preferedClubId, memberMoney);
            MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
            printf("[myId: %d][clock: %d][to:   %d] Odrzucenie zaproszenia do grupy {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
        }

        //4 i //9
        if ((myStatus == LEADER_STATUS || myStatus == ALONE_STATUS)  && recvMsg.message == CONFIRM_JOIN_MSG)
        {
            groupMoney += recvMsg.memberMoney;
            *(askTab + recvMsg.memberId) = ACCEPT_ASK_TAB;
            myStatus = ACCEPT_INVITATION_STATUS;
            printf("[myId: %d][clock: %d][from: %d] dołączył do grupy! Mamy teraz %d na %d pieniedzy {%d}\n", memberId, localClock, recvMsg.memberId, groupMoney, entryCost, myStatus);
        }

        //5
        if (myStatus == LEADER_STATUS && recvMsg.message == REJECT_JOIN_MSG)
        {
            *(askTab + recvMsg.memberId) = REJECT_ASK_TAB;
            myStatus = REJECT_INVITATION_STATUS;
            printf("[myId: %d][clock: %d][from: %d] Odrzucenie propozycji dołączenia do grupy {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
        }

        //6
        if (myStatus == ENOUGH_MONEY_STATUS && recvMsg.message == AGREE_TO_ENTER_CLUB_MSG)
        {
            approveCount++;
            printf("[myId: %d][clock: %d][from: %d] Pozwolenie dla mnie na wejscie do klubu %d {%d}\n", memberId, localClock, recvMsg.memberId, preferedClubId, myStatus);
            if (approveCount == noMembers - 1)
            {
                myStatus = ENTER_CLUB_STATUS;
            }
        }

        //7
        if (myStatus != ENOUGH_MONEY_STATUS && myStatus != ENTER_CLUB_STATUS && recvMsg.message == ASK_TO_ENTER_CLUB_MSG)
        {
            localClock++;
            sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
            MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
            printf("[myId: %d][clock: %d][to:   %d] Pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, recvMsg.memberId, recvMsg.preferedClubId, myStatus);
        }

        //8 TODO what that?!?! if elses every time localClock will be higher than recvMsg.localClock becouse we take max() + 1 to localClock
        if ((myStatus == ALONE_STATUS || myStatus == GROUP_BREAK_STATUS) && recvMsg.message == ASK_TO_JOIN_MSG)
        {
            if (recvMsg.localClock < localClock)
            {
                myStatus = MEMBER_STATUS;
                localClock++;
                sendMsg = createPackage(localClock, CONFIRM_JOIN_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Akceptuje zaproszenie do grupy {%d} \n", memberId, localClock, recvMsg.memberId, myStatus);
            }
            else
            {
                localClock++;
                sendMsg = createPackage(localClock, REJECT_JOIN_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Odrzucam zaproszenie do grupy {%d} \n", memberId, localClock, recvMsg.memberId, myStatus);
            }
        }


        //10 TODO why group break? could continue find another members?
        if (myStatus == ALONE_STATUS && recvMsg.message == REJECT_JOIN_MSG)
        {
            myStatus = GROUP_BREAK_STATUS;
            *(askTab + recvMsg.memberId) = REJECT_ASK_TAB;
            printf("[myId: %d][clock: %d][from: %d] Moje zaproszenie zostalo odrzucone {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
        }

        //11
        if (myStatus == MEMBER_STATUS && recvMsg.message == CONFIRM_JOIN_MSG)
        {
            localClock++;
            sendMsg = createPackage(localClock, GROUP_BREAK_MSG, memberId, preferedClubId, memberMoney);
            MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
            printf("[myId: %d][clock: %d][from: %d] Zrywam grupe (moje zaproszenie jest juz nie aktualne) {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
        }

        //12
        if (myStatus == MEMBER_STATUS && recvMsg.message == GROUP_BREAK_MSG)
        {
            myStatus = GROUP_BREAK_STATUS;
            printf("[myId: %d][clock: %d][from: %d] Grupa zostala rozwiazana {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
        }

        //13
        if (recvMsg.message == EXIT_CLUB_MSG)
        {
            preferedClubId = recvMsg.preferedClubId;
            printf("[myId: %d][clock: %d][from: %d] Wychodze z klubu jako czlonek grupy! Nr klubu: %d {%d}\n", memberId, localClock, recvMsg.memberId, preferedClubId, myStatus);
            myStatus = EXIT_CLUB_STATUS;
        }

        //14
        if (myStatus == ENTER_CLUB_STATUS && recvMsg.message == ASK_TO_ENTER_CLUB_MSG)
        {
            if (recvMsg.preferedClubId != preferedClubId)
            {
                localClock++;
                sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, recvMsg.memberId, recvMsg.preferedClubId, myStatus);
            }
        }
    }
    pthread_exit(NULL);
}

void createThread()
{
    pthread_t pthreadFunc;
    if (pthread_create(&pthreadFunc, NULL, threadFunc, NULL))
    {
        printf("[myId: %d]Error creating thread {%d}\n", memberId, myStatus);
        free(askTab);
        MPI_Type_free(&mpiMsgType);
        MPI_Finalize();
        exit(-1);
    }
}

void initMember()
{
    if (threadMoneyFlag)
    {
        threadMoneyFlag = false;
    }
    else
    {
        memberMoney = (rand() % (entryCost - 2)) + 1;
        printf("[myId: %d][clock: %d]           my memberMoney = %d\n", memberId, localClock, memberMoney);
    }
    groupMoney = memberMoney;
    myStatus = ALONE_STATUS;
    preferedClubId = rand() % noClubs;
    askTab = calloc(noMembers, sizeof(int));
    approveCount = 0;

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

void mainLoop()
{
    //START COMMUNICATE AFTER RANDOM SHORT TIME
    localClock = rand() % noMembers;
    sleep(localClock);

    msg sendMsg;
    bool restart;

    while (true)
    {
        initMember();
        restart = false;

        while (isNotEmptyTab())
        {
            //LEADERS or ALONE STATUS
            if (myStatus != MEMBER_STATUS)
            {
                //INCREASE LAMPORT CLOCK
                localClock++;

                //SELECT MEMBER TO ASK FROM askTab
                int selectedMember = getRandomMemberID();
                if (selectedMember == -1)
                {
                    printf("[myId: %d] ERROR: Selected member == -1 {%d}\n", memberId, myStatus);
                    break;
                }

                //SEND MESSAGE TO SELECTED MEMBER
                sendMsg = createPackage(localClock, ASK_TO_JOIN_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, selectedMember, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to: %d]   Zapytanie o dolaczenie do grupy {%d}\n", memberId, localClock, selectedMember, myStatus);
            }

            //WAIT FOR STATUS UPDATE - RECIEVE RESPONSE MSG /
            while (myStatus == ALONE_STATUS || myStatus == MEMBER_STATUS || myStatus == LEADER_STATUS)
            {
                //waiting for myStatus update
            }

            //EXIT ASKING, ENOUGH MONEY TO ENTER CLUB
            bool exitWhile = false;

            switch (myStatus)
            {
            //TRANSITION STATE AFTER RECV ACCEPTED INVITE REQ
            case ACCEPT_INVITATION_STATUS:
                myStatus = LEADER_STATUS;
                if (groupMoney >= entryCost)
                {
                    printf("[myId: %d][clock: %d]           Zebraliśmy pieniądze: %d na %d. Wybieramy klub. {%d}\n", memberId, localClock, groupMoney, entryCost, myStatus);
                    exitWhile = true;
                }
                break;
            
            //TRANSITION STATE AFTER RECV REJECT INVITE REQ
            case REJECT_INVITATION_STATUS:
                myStatus = LEADER_STATUS;
                break;

            //WHEN LEADER SEND BREAK_MSG - NOT ENOUGH MONEY
            case GROUP_BREAK_STATUS:
                groupMoney = memberMoney;
                myStatus = ALONE_STATUS;
                // TODO restart or not???
                break;

            //TODO - WHAT HAPPENING HERE
            case EXIT_CLUB_STATUS:
                printf("[myId: %d][clock: %d]           Wychodzę z klubu [%d] {%d}\n", memberId, localClock, preferedClubId, myStatus);
                restart = true;
                exitWhile = true;
                break;
            }

            if (exitWhile)
            {
                break;
            }
        }
        if (!restart)
        {
            //Jeżeli za mało pieniędzy oznacza że zapytał wszystkich i nie da rady więc rozwiązuje grupę
            if (groupMoney < entryCost && myStatus == LEADER_STATUS)
            {
                for (int i = 0; i < noMembers; i++)
                {
                    if (*(askTab + i) == ACCEPT_ASK_TAB && i != memberId)
                    {
                        localClock++;
                        sendMsg = createPackage(localClock, GROUP_BREAK_MSG, memberId, preferedClubId, memberMoney);
                        MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD); //Wyślij do wszystkich którzy są w mojej grupie (oprócz mnie)
                        printf("[myId: %d][clock: %d]        Rozwiazanie grupy dla RANK: %d {%d}\n", memberId, localClock, i, myStatus);
                    }
                }
            }

            //Jeżeli mamy siano i możemy ubiegać się o wejście
            if (groupMoney >= entryCost && myStatus == LEADER_STATUS)
            {
                printf("[myId: %d][clock: %d]           Wybieramy klub! {%d}\n", memberId, localClock, myStatus);
                myStatus = ENOUGH_MONEY_STATUS;
                preferedClubId = rand() % noClubs;
                printf("[myId: %d][clock: %d]           Wybralismy klub o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);
                for (int i = 0; i < noMembers; i++)
                {
                    if (i != memberId)
                    {
                        localClock++;
                        sendMsg = createPackage(localClock, ASK_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                        MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD); //Wyślij do wszystkich zapytanie o wejście do klubu
                        printf("[myId: %d][clock: %d]           Zapytanie o wejscie do klubu o nr: %d dla RANK: %d {%d}\n", memberId, localClock, preferedClubId, i, myStatus);
                    }
                }
                printf("[myId: %d][clock: %d]           Czekamy na pozwolenia na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);
                while (myStatus != ENTER_CLUB_STATUS)
                {
                    //waiting for perrmisions to go to club
                }

                printf("[myId: %d][clock: %d]       Mamy pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);
                if (myStatus == ENTER_CLUB_STATUS)
                {
                    for (int i = 0; i < noMembers; i++)
                    {
                        if (*(askTab + i) == ACCEPT_ASK_TAB && i != memberId)
                        {
                            localClock++;
                            sendMsg = createPackage(localClock, EXIT_CLUB_MSG, memberId, preferedClubId, memberMoney);
                            MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD); //Wyślij do wszystkich którzy są w mojej grupie info o wyjściu z klubu
                            printf("[myId: %d][clock: %d]        Informacja --> Koniec imprezy dla RANK: %d {%d}\n", memberId, localClock, i, myStatus);
                        }
                    }
                    for (int i = 0; i < noMembers; i++)
                    {
                        if (i != memberId && *(askTab + i) != ACCEPT_ASK_TAB)
                        {
                            localClock++;
                            sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                            MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD); //Wyślij do wszystkich info o możliwości wejścia do klubu w którym byliśmy
                            printf("[myId: %d][clock: %d]        Pozwolenie na wejscie do naszego klubu (nr: %d) dla RANK: %d {%d}\n", memberId, localClock, preferedClubId, i, myStatus);
                        }
                    }
                }

                printf("[myId: %d][clock: %d]        Kapitan wychodzi z klubu o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);

            }
            //EXIT CLUBS IN DIFFERERNT TIME
            int sleepTime = rand() % noMembers;
            localClock += sleepTime;
            sleep(sleepTime);
            printf("[myId: %d][clock: %d]           Spałem %d sekund\n", memberId, localClock, sleepTime);

        }
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
    offsets[3] = offsetof(msg, preferedClubId);
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
