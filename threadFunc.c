#include "main.h"

void *threadFunc()
{
    msg sendMsg;
    msg recvMsg;

    while (true)
    {
        MPI_Recv(&recvMsg, 1, mpiMsgType, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        localClock = max(recvMsg.localClock, localClock) + 1;

        switch (recvMsg.message)
        {
        case ASK_TO_JOIN_MSG:
            if (myStatus != ALONE_STATUS && myStatus != GROUP_BREAK_STATUS)
            {
                localClock++;
                sendMsg = createPackage(localClock, REJECT_JOIN_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Odrzucenie zaproszenia do grupy {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
            }
            else if (myStatus == ALONE_STATUS || myStatus == GROUP_BREAK_STATUS)
            {
                myStatus = MEMBER_STATUS;
                localClock++;
                sendMsg = createPackage(localClock, CONFIRM_JOIN_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Akceptuje zaproszenie do grupy {%d} \n", memberId, localClock, recvMsg.memberId, myStatus);
            }
            break;

        case CONFIRM_JOIN_MSG:
            if (myStatus == LEADER_STATUS || myStatus == ALONE_STATUS)
            {
                groupMoney += recvMsg.memberMoney;
                askTab[recvMsg.memberId] = ACCEPT_ASK_TAB;
                myStatus = ACCEPT_INVITATION_STATUS;
                ++approveCount;
                printf("[myId: %d][clock: %d][from: %d] Dołączył do grupy! Mamy teraz %d na %d pieniedzy {%d}\n", memberId, localClock, recvMsg.memberId, groupMoney, entryCost, myStatus);
            }
            else if (myStatus == MEMBER_STATUS)
            {
                localClock++;
                sendMsg = createPackage(localClock, GROUP_BREAK_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][from: %d] Zrywam grupe (moje zaproszenie jest juz nie aktualne) {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
            }
            break;

        case REJECT_JOIN_MSG:
            if (myStatus == LEADER_STATUS || myStatus == ALONE_STATUS)
            {
                askTab[recvMsg.memberId] = REJECT_ASK_TAB;
                myStatus = REJECT_INVITATION_STATUS;
                printf("[myId: %d][clock: %d][from: %d] Odrzucenie propozycji dołączenia do grupy {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
            }
            break;

        case GROUP_BREAK_MSG:
            if (myStatus == MEMBER_STATUS)
            {
                myStatus = GROUP_BREAK_STATUS;
                printf("[myId: %d][clock: %d][from: %d] Grupa zostala rozwiazana {%d}\n", memberId, localClock, recvMsg.memberId, myStatus);
            }
            break;

        case ASK_TO_ENTER_CLUB_MSG:
            if (myStatus == ENOUGH_MONEY_STATUS)
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
            else if (myStatus != ENOUGH_MONEY_STATUS && myStatus != ENTER_CLUB_STATUS)
            {
                localClock++;
                sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                printf("[myId: %d][clock: %d][to:   %d] Pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, recvMsg.memberId, recvMsg.preferedClubId, myStatus);
            }
            else if (myStatus == ENTER_CLUB_STATUS)
            {
                if (recvMsg.preferedClubId != preferedClubId)
                {
                    localClock++;
                    sendMsg = createPackage(localClock, AGREE_TO_ENTER_CLUB_MSG, memberId, preferedClubId, memberMoney);
                    MPI_Send(&sendMsg, 1, mpiMsgType, recvMsg.memberId, MSG_TAG, MPI_COMM_WORLD);
                    printf("[myId: %d][clock: %d][to:   %d] Pozwolenie na wejscie do klubu o nr: %d {%d}\n", memberId, localClock, recvMsg.memberId, recvMsg.preferedClubId, myStatus);
                }
            }
            break;

        case AGREE_TO_ENTER_CLUB_MSG:
            if (myStatus == ENOUGH_MONEY_STATUS)
            {
                approveCount++;
                printf("[myId: %d][clock: %d][from: %d] Pozwolenie dla mnie na wejscie do klubu %d {%d}\n", memberId, localClock, recvMsg.memberId, preferedClubId, myStatus);
                if (approveCount == noMembers - 1)
                {
                    myStatus = ENTER_CLUB_STATUS;
                }
            }
            break;

        case EXIT_CLUB_MSG:
            preferedClubId = recvMsg.preferedClubId;
            printf("[myId: %d][clock: %d][from: %d] Wychodze z klubu jako czlonek grupy! Nr klubu: %d {%d}\n", memberId, localClock, recvMsg.memberId, preferedClubId, myStatus);
            myStatus = EXIT_CLUB_STATUS;
            break;
        }
        pthread_cond_signal(&cond1);
    }
    pthread_exit(NULL);
}
