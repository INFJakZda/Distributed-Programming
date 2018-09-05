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
            pthread_mutex_lock(&lock);
            while (myStatus == ALONE_STATUS || myStatus == MEMBER_STATUS || myStatus == LEADER_STATUS)
            {
                //WAIT FOR UPDATE FROM OTHER THREAD
                pthread_cond_wait(&cond1, &lock);
            }
            pthread_mutex_unlock(&lock);

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
        if ((!initAgain) && (myStatus == LEADER_STATUS)) { leader_func(sendMsg); }
    }
}
