#ifndef RACE_SIMULATOR_H
#define RACE_SIMULATOR_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <regex.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#define PATTERN_ADD "[:number:][:space:]$"



#define LOG_FILE "log.txt"

#define CONFIG_FILE "config.txt"

#define PIPE "input_pipe"

#define BUF_SIZE 1024


#define START_RACE 2
#define ADDCAR 1
#define ERRO 0
#define EXIT 3


#define MAX_LEN_NAME 100

typedef struct
{
    int time_unit;
    int lap_distance; //metros
    int number_of_laps;
    int number_of_teams;
    int cars_per_team;
    int T_avaria;
    int T_Box_min;
    int T_Box_max;
    int fuel_tank;
}config_struct;




// Car struct
typedef struct{

    pthread_t thread;

    int slot_id, id;

    char name[MAX_LEN_NAME];
    int estado; //1-Corrida 2-Segurança 3-Box 4-Desistencia 5-Terminado
    
    int speed;
    int consumption;
} car;

typedef struct
{
    
    int totalAvarias;
    int totalAbastecimentos;
    int carrosEmPista;
    car* lastPosition;

}stats_struct;

// Cars structs

typedef struct
{
    int slot_id;        //slot shm
    int estadoBox; //1-ocupado 0-livre
    int numeroCarros;
    car* arrayCarros;
    int *ids;
    int id;
    char nome[10];
}team;


typedef struct 
{
    stats_struct stats; //--> mais para a frente


    pthread_mutex_t mutex;

    //array equipas
    team *arrayEquipas;

    //array de carros em pista
    car* cars;

}shm_struct;

typedef struct Node* Node;
typedef struct Node{
    void* cargo;
    Node previous, next;
} tnode;

#include "race_manager.h"
#include "teamManager.h"
#include "malfunction_manager.h"

void write_log(char *text);
void init();
void terminate();
void read_config();
void estatisticas();

#endif