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

//Ler ficheiro config
void read_config(){
    
    FILE *config_file = fopen(CONFIG_FILE, "r");

    if(config_file == NULL){
        printf("Erro ao abrir o ficheiro\n"); // colocar no log
    }
    
    
    fscanf(config_file, "%d\n", &config->time_unit);

    
    fscanf(config_file, "%d, %d\n", &config->lap_distance, &config->number_of_laps);
    fscanf(config_file, "%d\n", &config->number_of_teams);
    if(config->number_of_teams < 3){
        printf("Não atinge o mínimo de equipas. Ficheiro Rejeitado\n"); // colocar no log
    }
    fscanf(config_file, "%d\n", &config->cars_per_team);
    fscanf(config_file, "%d\n", &config->T_avaria);
    fscanf(config_file, "%d, %d\n", &config->T_Box_min, &config->T_Box_max);
    fscanf(config_file, "%d\n", &config->fuel_tank);

    fclose(config_file);
}



void race_sim_init(){

    for(int i = 0; i < config->number_of_teams; i++){
        shm->arrayEquipas[i].slot_id = i;

    }

}

void race_sim_terminate(){


    
}

//"main"
void race_sim(config_struct *_config, shm_struct *_shm){
    config = _config;
    shm = _shm;

    write_log("Simulation Manager starting");
    printf("Simulation Manager starting on PID: %d\n", getpid());

    

    race_sim_init();

    
    
    if(fork() == 0){
        
        race_manag(config,shm);
        //wait(NULL);
    }
    else{
        if(fork() == 0){
            malfunc_manager(config);
        }
        else{
            wait(NULL);
        }
    }
    

}