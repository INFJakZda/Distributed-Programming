#ifndef MAIN_H
#define MAIN_H

#include <mpi.h>
#include <stdio.h>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

#include "Pensioner.h"
#include "Lamport.hpp"

#define ROOT 0
#define MSG_TAG 100
#define PENSIONERS_NR 20
#define CLUB_SIZE 50
#define ENTRY_AMOUNT 100

#define MSG_TAG_COMMUNICATION 100
#define MSG_TAG_TEAM_COMMUNICATION 200

#endif
