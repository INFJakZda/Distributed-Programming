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
    N – noMembers - liczba emerytów
    M – entryAmount - ilość pieniędzy potrzebna do wstępu do klubu
    K - noClubs - liczba klubów, N >> K
    memberMoney – kwota którą otrzymał emeryt od organizacji
    groupMoney – kwota jaką posiada grupa
    askTab[N] – tablica reprezentująca aktualny status innych emerytów
        0 – emeryt gotowy do zapytania
        1 – zaakceptował zapytanie (w grupie)
        2 – odrzucił zapytanie (w innej grupie)

## Init:
    1. Każdy z emerytów dostaje losową kwotę pieniędzy [retireeMoney] po losowym czasie, w zakresie od 1 do M (entryAmount).
    2. Ustawienie wartości tablicy [askTab] na 0 (gotowy do zapytania). 
    3. myStatus = Brak_Przydzielonej_Grupy.
    4. groupMoney = 0
## Wątek główny:   

## Wątek odbierający wiadomości:    

## Złożoność
