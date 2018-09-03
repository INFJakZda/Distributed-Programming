#include "main.h"

void mainLoop()
{
    //START COMMUNICATE AFTER RANDOM SHORT TIME
    int sleepTime = rand() % (noMembers / 2);
    localClock += sleepTime;
    //sleep(localClock);
    //printf("[myId: %d][clock: %d]           Obudził się po %d czasu\n", memberId, localClock, sleepTime);

    msg sendMsg;

    //WHEN NEED TO RESET VALUES OF askTab etc.
    bool initAgain;
    //EXIT ASKING, ENOUGH MONEY TO ENTER CLUB OR WHEN GROUP BREAK
    bool exitWhile;

    while (true)
    {
        initMember();
        initAgain = false;

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
                //WAIT FOR UPDATE FROM OTHER THREAD
            }

            //EXIT ASKING, ENOUGH MONEY TO ENTER CLUB
            exitWhile = false;

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
                exitWhile = true;
                initAgain = true;
                break;

            //WHEN MEMBER AND RECIEVE EXIT CLUB MSG
            case EXIT_CLUB_STATUS:
                printf("[myId: %d][clock: %d]           Wychodzę z klubuu [%d] {%d}\n", memberId, localClock, preferedClubId, myStatus);
                initAgain = true;
                exitWhile = true;
                break;
            }

            if (exitWhile)
            {
                break;
            }
        }
        if (!initAgain)
        {
            //WHEN HAS NO ENOUGH MONEY - GROUP BREAK
            if (groupMoney < entryCost && myStatus == LEADER_STATUS)
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
            else if (groupMoney >= entryCost && myStatus == LEADER_STATUS)
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
                while (myStatus != ENTER_CLUB_STATUS)
                {
                    //WAIT FOR UPDATE FROM OTHER THREAD
                }

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

            //EXIT CLUBS IN DIFFERERNT TIME
            //int sleepTime = rand() % (noMembers / 2);
            //localClock += sleepTime;
            //sleep(sleepTime);
            //printf("[myId: %d][clock: %d]           Spałem %d czasu\n", memberId, localClock, sleepTime);
        }
    }
}
