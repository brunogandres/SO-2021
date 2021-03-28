/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */
#include "main.h"

config_struct config;

void race_manag(){
    write_log("Race Manager starting");
    printf("Race Manager starting on PID: %d AND FATHER IS %d\n", getpid(), getppid());

    for(int i= 0; i < config.number_of_teams; i++){
        if(fork()){
            wait(NULL);
        }
        else{
            team_man();
            exit(0);
        }
    }
}