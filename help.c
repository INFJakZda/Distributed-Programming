#include "main.h"

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
        askTab[i] = READY_ASK_TAB;
        if (i == memberId)
            askTab[i] = ACCEPT_ASK_TAB;
    }
}

bool isNotEmptyTab()
{
    for (int i = 0; i < noMembers; i++)
    {
        if (askTab[i] == 0)
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
