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
        2. CONFIRM_JOIN_MSG
        3. REJECT_JOIN_MSG
        4. GROUP_BREAK_MSG
        5. ASK_TO_ENTER_CLUB_MSG
        6. AGREE_TO_ENTER_CLUB_MSG
        7. EXIT_CLUB_MSG

        1. Jeżeli status jest ENOUGH_MONEY i wiadomość odebrana to ENTER_CLUB_QUERY
            a. Jeżeli inny klub niż nasz wybrany
                i. Wyślij zgodę na wejście do klubu
            b. Jeżeli nasz
                i. Jeżeli ich zegar jest większy od naszego wyślij zgodę
        2. Jeżeli status jest inny od NO_GROUP i GROUP_BREAK oraz odebrana wiadomość  to GROUP_INVITE
            a. Wyślij odrzucenie zaproszenia do grupy
        3. Jeżeli status FOUNDER i odebrana wiadomość to GROUP_CONFIRMATION
            a. Dodaj do groupMoney kwotę emeryta którego przyjmujemy
            b. Zmień w tab pozycje danego emeryta na MY_GROUP
        4. Jeżeli status FOUNDER i wiadomość to REJECT_INVITE_MSG
            a. Oznacz w tab pozycje danego emeryta na NOT_MY_GROUP
            b. Ustaw status na REJECT_INVITE
        5. Jeżeli status ENOUGH_MONEY i wiadomość to ENTER_PERMISSION
            a. Zwiększ approveCount
            b. Jeżeli approveCount == N-1
                i. Ustaw status na ENTER_CLUB
        6. Jeżeli status jest inny od ENOUGH_MONEY i ENTER_CLUB oraz odebrana wiadomość to ENTER_CLUB_QUERY
            a. Wyślij zgodę na wejście do klubu
        7. Jeżeli status jest NO_GROUP lub GROUP_BREAK oraz wiadomość to GROUP_INVITE
            a. Jak zegar odebrany jest mniejszy od naszego
                i. Ustaw status na PARTICIPATOR
                ii. Wyślij akceptacje zaproszenia
            b. Jeśli zegar większy odrzuć zaproszenie
        8. Jak status NO_GROUP i wiadomość to GROUP_CONFIRMATION
            a. Zmień status na ACCEPT_INVITE
            b. Oznacz w tab pozycje danego emeryta na MY_GROUP
            c. Zwiększ groupMoney o kwotę zaakceptowanego emeryta
        9. Jeżeli status NO_GROUP i odebrana wiadomość to REJECT_INVITE_MSG
            a. Ustaw status na GROUP_BREAK
            b. W tablicy tab ustaw pozycje danego emeryta na NOT_MY_GROUP
        10. Jeżeli status PARTICIPATOR i odebrana wiadomość GROUP_CONFIRMATION
            a. Odrzuć zaproszenie, bo należę do innej grupy
        11. Jeżeli status PARTICIPTOR i odebrana wiadomość to GROUP_BREAK_MSG
            a. Ustaw status na GROUP_BREAK
        12. Jeżeli odebrana wiadomość EXIT_CLUB_MSG
            a. Ustaw clubNumber = odebrany numer klubu
            b. Ustaw status na EXIT_CLUB
        13. Jeżeli status ENTER_CLUB i odebrana wiadomość ENTER_CLUB_QUERY
            a. Jeżeli różne numery klubów to wyślij pozwolenie na wejście do klubu


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
