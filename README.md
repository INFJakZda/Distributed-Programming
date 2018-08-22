# Distributed-Programming
Academic project using distributed programming.

## Task details
A certain organization of retirees from time to time draws a small amount for its members. Members for this amount are going to clubs. There are K clubs and admission to each of them costs the same but they offer different attractions. Unfortunately the cost of admission is always payable on groups regardless of their size. At the same time there can only be one group in the club. The cost is more than the largest amount given to one member so members must get into groups to collect the amount needed to pay for admission. Members also have to agree to which particular club they will go. If the sum collected by pensioners is greater than M the members will enter to the club.

## Restrictions
1. There are N retirees, K clubs, N >> K
2. Cost to enter to every club is M
3. Every retireer gets random from 1 to M-1 amount of money
4. Every club is unique and irreplaceable

# Algorytm do problemu

## Zmienne:
    noMembers   - N - liczba emerytów
    noClubs     - K - liczba klubów, N >> K
    entryAmount - M - kwota potrzebna do wstępu do klubu    
    memberMoney - kwota którą otrzymał emeryt od organizacji
    groupMoney - kwota jaką posiada grupa
    askTab[N] - tablica reprezentująca aktualny status innych emerytów:
        0 - READY_ASK_TAB - emeryt gotowy do zapytania
        1 - ACCEPT_ASK_TAB - zaakceptował zapytanie (w grupie)
        2 - REJECT_ASK_TAB - odrzucił zapytanie (w innej grupie)
    myStatus - status emeryta: 
        0 - ALONE_STATUS - pojedynczy emeryt (bez grupy)
        1 - LEADER_STATUS - założyciel grupy
        2 - MEMBER_STATUS - uczestnik grupy

## Init:
    1. Każdy z emerytów dostaje losową kwotę pieniędzy [memberMoney] po losowym czasie, w zakresie od 1 do M (entryAmount).
    2. Ustawienie wartości tablicy [askTab] na 0 (gotowy do zapytania). 
    3. myStatus = 0.
    4. groupMoney = 0
## Wątek główny:   
    Dopóki: askTab zawiera 0 - gotowy do zapytania:
        1. Wyślij zapytanie o dołączenie do grupy do dowolnego emeryta z wartością 0 w askTab.
        2. 

## Wątek odbierający wiadomości:
    Odbierz wiadomość [message] od dowolnego odbiorcy:
        1. Jeżeli (myStatus == 0 [pojedynczy emeryt] oraz message == GROUP_INVITATION)


## Złożoność