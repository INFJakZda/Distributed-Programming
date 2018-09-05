#include "main.h"

void leader_func(msg sendMsg)
{
    //WHEN HAS NO ENOUGH MONEY - GROUP BREAK
    if (groupMoney < entryCost)
    {
        for (int i = 0; i < noMembers; i++)
        {
            if (askTab[i] == ACCEPT_ASK_TAB && i != memberId)
            {
                localClock++;
                sendMsg = createPackage(localClock, GROUP_BREAK_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Rozwiazanie grupy {%d}\n", memberId, localClock, i, myStatus);
            }
        }
    }
    //WHEN HAS ENOUGH MONEY - ENTER TO CLUB
    else if (groupMoney >= entryCost)
    {
        myStatus = ENOUGH_MONEY_STATUS;
        printf("[myId: %d][clock: %d] MAMY KLUB Wybralismy klub o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);
        //ASK EVERYONE - NOT IN MY GROUP
        for (int i = 0; i < noMembers; i++)
        {
            if ((i != memberId) && (askTab[i] != ACCEPT_ASK_TAB))
            {
                localClock++;
                sendMsg = createPackage(localClock, ASK_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Pytam sie o pozwolenie do klubu: %d {%d}\n", memberId, localClock, i, preferedClubId, myStatus);
            }
        }
        printf("[myId: %d][clock: %d]           Wysłaliśmy wszytskim zapytanie o wejscie do klubu: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);
        pthread_mutex_lock(&lock);
        while (myStatus != ENTER_CLUB_STATUS)
        {
            //WAIT FOR UPDATE FROM OTHER THREAD
            pthread_cond_wait(&cond1, &lock);
        }
        pthread_mutex_unlock(&lock);
        printf("[myId: %d][clock: %d] WEJSCIE DO KLUBU Wchodzimy do klubu o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);

        //WHEN ENTER CLUB - SEND MSG TO MEMBERS THAT WE EXIT CLUB
        for (int i = 0; i < noMembers; i++)
        {
            if (askTab[i] == ACCEPT_ASK_TAB && i != memberId)
            {
                localClock++;
                sendMsg = createPackage(localClock, EXIT_CLUB_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Wychodzimy z klubu {%d}\n", memberId, localClock, i, myStatus);
            }
        }
        //SEND MSG AFTER CLUBING THAT WE EXIT CLUB
        for (int i = 0; i < noMembers; i++)
        {
            if (i != memberId && askTab[i] != ACCEPT_ASK_TAB)
            {
                localClock++;
                sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, i, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Info o zwolnieniu klubu nr: %d {%d}\n", memberId, localClock, i, preferedClubId, myStatus);
            }
        }

        printf("[myId: %d][clock: %d]        Kapitan wychodzi z klubu o nr: %d {%d}\n", memberId, localClock, preferedClubId, myStatus);
    }
}
