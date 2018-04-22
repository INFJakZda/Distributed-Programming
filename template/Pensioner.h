#ifndef PENSIONER_H
#define PENSIONER_H

class Pensioner {
    
    private bool* clubArray;
    private unsigned int moneyAmount;
    private bool isLeader;
    private unsigned int groupMoney;

    public Pensioner() {
        clubArray = new bool[CLUB_SIZE];
    }


    public void grant_money();
    public void listen();
    public void asking();

}
#endif