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

config_struct *config;
shm_struct *shm;

Node header;

pthread_mutexattr_t attrmutex;


void race_manag_init(){

    header = linked_list_create();

    

}


void race_manag(config_struct *_config, shm_struct *_shm){
    config = _config;
    shm = _shm;
    write_log("Race Manager starting");
    printf("Race Manager starting on PID: %d AND FATHER IS %d\n", getpid(), getppid());

    race_manag_init();
    for(int i= 0; i < config->number_of_teams; i++){
        if(fork()){
            wait(NULL);
        }
        else{
            team_man(i, config, shm, header);
            
            exit(0);
        }
    }

    //for(int i=0;i<config->number_of_teams;i++) wait(NULL);
}