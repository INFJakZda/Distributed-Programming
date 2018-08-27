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
### 1. TODO
* Przy pierwszym połączeniu w głównym wątku nie ma obsłużenia tego statusu
* Czy chcemy mieć możliwość połączenia się dwóch grup czy nie?
    * Jeśli nie to rozwiaznie istnieje
    * Jeśli tak to musimy przekazywać jakich emerytów mamy w grupie
*
## Zmienne:
    noMembers   - N - liczba emerytów
    noClubs     - K - liczba klubów, N >> K
    entryCost - M - kwota potrzebna do wstępu do klubu
    memberMoney - kwota którą otrzymał emeryt od organizacji
    groupMoney - kwota jaką posiada grupa
    memberId - unikalny identyfikator emeryta
    preferedClubId - preferowane ID clubu z góry wylosowane
    localClock - lokalny zegar logiczny Lamporta
    askTab[N] - tablica reprezentująca aktualny status innych emerytów:
        0 - READY_ASK_TAB - emeryt gotowy do zapytania
        1 - ACCEPT_ASK_TAB - zaakceptował zapytanie (w grupie)
        2 - REJECT_ASK_TAB - odrzucił zapytanie (w innej grupie)
    myStatus - status emeryta:
        0 - ALONE_STATUS - pojedynczy emeryt (bez grupy)
        -1 - WAIT_FOR_RESPONSE_STATUS - czekanie na potwierdzenie od konkretnego emeryta
        1 - LEADER_STATUS - założyciel grupy
        2 - MEMBER_STATUS - uczestnik grupy
    message - typ wiadomości przesyłanych pomiędzy emerytami
        0 - ASK_TO_JOIN_MSG - zapytanie o dołączenie do grupy
        1 - CONFIRM_JOIN_MSG 1 - potwierdzenie dołączenia do grupy
        2 - REJECT_JOIN_MSG 2 - odrzucenie propozycji dołączenia do grupy
        3 - DISSOLUTION_GROUP_MSG 3
        4 - ASK_TO_ENTER_MSG 4
        5 - AGREE_TO_ENTER_CLUB_MSG 5
        6 - DISAGREE_TO_ENTER_CLUB_MSG 6
        7 - EXIT_CLUB_MSG 7


## Init:
    1. Każdy z emerytów dostaje losową kwotę pieniędzy [memberMoney] po losowym czasie, w zakresie od 1 do M (entryCost).
    2. Ustawienie wszystkich wartości tablicy [askTab] na 0 (gotowy do zapytania).
    3. myStatus = 0 [ALONE_STATUS].
    4. groupMoney = 0

## Wątek główny:
    Dopóki: askTab zawiera 0 - gotowy do zapytania:
        1. Wyślij zapytanie o dołączenie do grupy [ASK_TO_JOIN_MSG] do dowolnego emeryta z wartością 0 [READY_ASK_TAB] w askTab.
            1.1. myStatus = WAIT_FOR_RESPONSE_STATUS
        2. Jeżeli( status = 1 [LEADER_STATUS]

## Wątek odbierający wiadomości:
    Odbierz wiadomość [message] od dowolnego odbiorcy:
        1. Jeżeli (myStatus == 0 [ALONE_STATUS] oraz message == 0 [ASK_TO_JOIN_MSG])
            1.1. status = 2 [MEMBER_STATUS] - uczestnik grupy
            1.2. wyślij odpowiedź z potwierdzeniem dołączenia = 1 [CONFIRM_JOIN_MSG]
        2. Jeżeli (myStatus == -1 [WAIT_FOR_RESPONSE_STATUS] oraz message == 1 [CONFIRM_JOIN_MSG])
            2.1. status = 1 [LEADER_STATUS]
            2.2. askTab[message.memberId] = 1 [ACCEPT_ASK_TAB]
            2.3. groupMoney += message.money + memberMoney;
        3. Jeżeli (myStatus == -1 [WAIT_FOR_RESPONSE_STATUS] oraz message == 2 [REJECT_JOIN_MSG])
            3.1. askTab[message.memberId] = 2 [REJECT_ASK_TAB]
        4. Jeżeli (myStatus != 0 [ALONE_STATUS] oraz message == 0 [ASK_TO_JOIN_MSG])
            4.1. wyślij odpowiedź z odrzuceniem dołączenia = 2 [REJECT_JOIN_MSG]
        5. Jeżeli (myStatus == -1 [WAIT_FOR_RESPONSE_STATUS] oraz message == 0 [ASK_TO_JOIN_MSG])
            5.1.

## Złożoność

## Zmienne pomocnicze
    status – aktualny stan emeryta
        0 – brak przydzielonej grupy
        1 – założyciel grupy
        -1 – uczestnik grupy ale nie założyciel
        2 – rozwiązywanie grupy (status pośredni)
        3 – pozwolenie na wejście do klubu
        4 – Wyjście z klubu, rozpoczęcie algorytmu od nowa
        5 – Emeryt zaakceptował zaproszenie do grupy
        6 – emeryt odrzucił zaproszenie do grupy
        7 – emeryci w grupie mają odpowiednią ilość pieniędzy i mogą wybierać klub
    tab[N] – tablica wypełniona „0” o wielkości N, reprezentująca emerytów
        0 – emeryt nie zapytany jeszcze
        1 – w mojej grupie
        -1 – emeryt odrzucił propozycje dołączenia do grupy
    Message – rodzaje wiadomości wysyłane między emerytami
        0 – zapytanie czy można wejść do danego klubu
        1 – zapytanie o dołączenie do grupy
        2 – potwierdzenie dołączenia do grupy
        3 – rozwiązanie grupy
        4 – Odrzucenie zaproszenia dołączenia do grupy
        5 – zgoda na wejście do sekcji od danego emeryta
        6 – Wyjście z klubu, zakończenie algorytmu
