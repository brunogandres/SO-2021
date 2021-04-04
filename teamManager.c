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

shm_struct *shm;// Shared memory
config_struct *config;// Config struct
Node header;
Node headerCars;




int carsTotal;
int idTeam;








void create_cars(){
    int r[carsTotal];
    int t;
    shm->arrayEquipas[idTeam].ids = r;
    for(int i = 0; i < carsTotal; i++){
        r[i] = i;
        //printf("ID CARRO: %d\n", shm->arrayEquipas[idTeam].arrayCarros[i].id);

        t = pthread_create(&shm->arrayEquipas[idTeam].arrayCarros[i].thread, NULL, thread_sim_car, &r[i]);
        if(t == 0)
            printf("[%s] Thread Carro crida com o id %d\n", shm->arrayEquipas[idTeam].nome,shm->arrayEquipas[idTeam].arrayCarros[i].id);
    }
    
    

}


void *thread_sim_car(void *carro){
    printf("entrou\n");
}

void team_man_init(){
    
    team *equipa;
    car carros[config->cars_per_team];

    headerCars = linked_list_create();

    equipa = malloc(sizeof(team));
    if(equipa == NULL){
        printf("Error on malloc\n");
        exit(-1);
    }

    char n[100];
    char buffer[20];
    equipa->slot_id = idTeam;
    
    strcat(n, "Equipa ");
    sprintf(buffer, "%d", idTeam);

    strcat(n, buffer);

    strcpy(equipa->nome,n );
    
    
    
    for(int i = 0; i < config->cars_per_team; i++){
        carros[i].id = i;
    }
    equipa->arrayCarros = carros;

    //shm->arrayEquipas[idTeam].arrayCarros = carros;
    
    shm->arrayEquipas[idTeam] = *equipa;

    
    linked_list_insert(header, equipa);
    
    
    
    





    create_cars();



}

void team_man_terminate(){

}



//main
void team_man(int id, config_struct *_config, shm_struct *_shm,Node _header){

    write_log("Team Manager starting");
    printf("Team Manager starting on PID: %d AND FATHER IS %d\n", getpid(), getppid());
    
    config = _config;
    idTeam = id;
    carsTotal = config->cars_per_team;
    header = _header;

    team_man_init();


}
