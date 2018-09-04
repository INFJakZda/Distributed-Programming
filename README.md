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
    entryCost   - M - kwota potrzebna do wstępu do klubu
    memberMoney - kwota którą otrzymał emeryt od organizacji
    groupMoney - kwota jaką posiada grupa
    memberId - unikalny identyfikator emeryta
    preferedClubId - preferowane ID clubu z góry wylosowane
    localClock - lokalny zegar logiczny Lamporta
    approveCount - zliczenie zezwoleń na wejście do klubu

    askTab[N] - tablica reprezentująca aktualny status innych emerytów:
        0 - READY_ASK_TAB - emeryt gotowy do zapytania
        1 - ACCEPT_ASK_TAB - zaakceptował zapytanie (w grupie)
        2 - REJECT_ASK_TAB - odrzucił zapytanie (w innej grupie)

    myStatus - status emeryta:
        0 - ALONE_STATUS - pojedynczy emeryt (bez grupy)
        1 - LEADER_STATUS - założyciel grupy
        2 - MEMBER_STATUS - uczestnik grupy
        3 - ACCEPT_INVITATION_STATUS - status pośredni - po odebraniu potwierdzenia dołączenia do grupy
        4 - REJECT_INVITATION_STATUS - status pośredni - po odebraniu odrzucenia dołączenia do grupy
        5 - ENOUGH_MONEY_STATUS - wystarczająca kwota do wstępu do klubu
        6 - ENTER_CLUB_STATUS - pozwolenie na wejście do klubu
        7 - EXIT_CLUB_STATUS - wyjście z klubu, algorytm od nowa
        8 - GROUP_BREAK_STATUS - rozwiązanie grupy

    message - typ wiadomości przesyłanych pomiędzy emerytami
        0 - ASK_TO_JOIN_MSG - zapytanie o dołączenie do grupy
        1 - CONFIRM_JOIN_MSG 1 - potwierdzenie dołączenia do grupy
        2 - REJECT_JOIN_MSG 2 - odrzucenie propozycji dołączenia do grupy
        3 - GROUP_BREAK_MSG 3
        4 - ASK_TO_ENTER_CLUB_MSG 4
        5 - AGREE_TO_ENTER_CLUB_MSG 5
        6 - EXIT_CLUB_MSG 6

## Wątek odbierający wiadomości:
    I. Odbierz wiadomość od dowolnego odbiorcy, w zależności od [message]:
        1. ASK_TO_JOIN_MSG
            Jeżeli status != ALONE lub GROUP_BREAK
                1.1. Wyślij odrzucenie dołączenia do grupy
            W przeciwnym przypadku 
                1.2. Wyślij przyjęcie propozycji dołączenia
                1.3. Zmień myStatus na MEMBER - uczestnik grupy

        2. CONFIRM_JOIN_MSG
            Jeżeli status == LEADER lub ALONE
                2.1. Dodaj do groupMoney kwotę emeryta którego przyjmujemy
                2.2. Zmień w askTab pozycje danego emeryta na ACCEPT
                2.3. Zmień status na ACCEPT_INVITATION
                2.4. Zinkrementuj approveCount
            Jeżeli status == MEMBER
                2.5. Wyślij info o zerwaniu grupy, nie aktualne zaproszenie

        3. REJECT_JOIN_MSG
            Jeżeli status == LEADER lub ALONE
                3.1. Zmień w askTab pozycje danego emeryta na REJECT
                3.2. Zmień status na REJECT_INVITATION

        4. GROUP_BREAK_MSG
            Jeżeli status == MEMBER
                4.1. Zmień status na GROUP_BREAK

        5. ASK_TO_ENTER_CLUB_MSG
            Jeżeli status == ENOUGH_MONEY
                5.1. Jeżeli różne numery klubów to wyślij pozwolenie na wejście do klubu
                5.2.

        6. AGREE_TO_ENTER_CLUB_MSG
            Jeżeli status == ENOUGH_MONEY
                6.1. Zinkrementuj approveCount
                6.2. Jeżeli approveCount == noMembers - 1
                    Zmień status na ENTER_CLUB

        7. EXIT_CLUB_MSG
            7.1. Odczytaj do jakiego klubu idziemy
            7.2. Zmień status na EXIT_CLUB

## Init Member:
    1. Każdy z emerytów dostaje losową kwotę pieniędzy [memberMoney] w zakresie < 1 ; (entryCost - 1) >.
    2. groupMoney = memberMoney
    3. myStatus = ALONE_STATUS.
    4. Losowe wybranie klubu które wybierze emeryt gdy będzie liderem grupy. [preferedClubId].
    5. Ustawienie wszystkich wartości tablicy [askTab] na READY_ASK_TAB.
        5.1. askTab[memberId] = ACCEPT_ASK_TAB.

## Wątek główny:
    I. Rozpoczęcie wątku po losowym czasie z zakesu < 0 ; noMembers >, co pozwoli na łączenie się w grupy, gdyż wątek odbierający wiadomości już nasłuchuje.
        1.1. localClock = wyslosowany czas (rozpoczęcia wątku).

    II. Init Member - wywołanie inicjalizacji - wyzerowanie zmiennych.

    III. Dopóki: askTab zawiera READY_ASK_TAB - gotowy do zapytania:
        1. Wyślij zapytanie o dołączenie do grupy [ASK_TO_JOIN_MSG] do dowolnego emeryta z wartością [READY_ASK_TAB] w askTab.
        2. Zaczekaj dopóki nie zmieni się stan z następujących stanów
            2.1. ALONE_STATUS - rozpoczęcie wątku, czekanie na odpowiedź zaproszenia do grupy,
            2.2. LEADER_STATUS - założyciel grupy, czekanie na odpowiedź,
            2.3. MEMBER_STATUS - uczestnik grupy, czekanie na wejście do klubu,
        3. Wykonaj odpowiednie operacje ze względu na myStatus:
            ACCEPT_INVITATION_STATUS:
                1. zmień myStatus na LEADER_STATUS
                2. Jeżeli groupMoney > entryCost
                    2.1. Idź do pkt. IV.
            REJECT_INVITATION_STATUS:
                1. zmień myStatus na LEADER_STATUS
            GROUP_BREAK_STATUS:
                1. groupMoney = memberMoney;
                2. zmień myStatus na ALONE_STATUS
                3. Idź do pkt II.
            EXIT_CLUB_STATUS:
                1. Idź do pkt II.
        Jeżeli tu jesteś to kontynuuj -> pkt III.1.

    IV. Obsługa decyzji liderów - wybór klubu / rozwiązanie grupy
        1. Jeżeli groupMoney < entryCost
            1.1 Wyślij każdemu z grupy (askTab == ACCEPT) info o rozwiązaniu grupy
        2. Jeżeli groupMoney > entryCost
            2.1. Spytaj się wszystkich spoza grupy czy moja grupa może wejść do klubu.
            2.2. Czekaj na pozwolenie od wszystkich zapytanych.
            2.3. Wyślij pozwolenie na wejście do klubu do wszystkich z mojej grupy.
            2.4. Wyślij wszystkim spoza grupy info o tym że moja grupa wyszła z klubu.

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
