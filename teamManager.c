/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */

#include "race_simulator.h"

shm_struct *shm;// Shared memory
config_struct *config;// Config struct
team *arrayEquipas;

Node header;
Node headerCars;
pthread_mutexattr_t attrmutex;






int carsTotal;
int idTeam;





void *thread_sim_car(void *carro){
    return 0;
}


void create_cars(){
    int r[carsTotal];
    int t;

    pthread_t temp;
    pthread_mutex_lock(&shm->mutex);
    arrayEquipas[idTeam].ids = r;
    
    
    for(int i = 0; i < carsTotal; i++){
        r[i] = i;
        printf("%d\n", i);
        t = pthread_create(&arrayEquipas[idTeam].arrayCarros[i].thread, NULL, thread_sim_car, &r[i]);
        if(t == 0)
            printf("[%s] Thread Carro crida com o id %d\n",arrayEquipas[idTeam].nome,arrayEquipas[idTeam].arrayCarros[i].id);

    }
    pthread_mutex_unlock(&shm->mutex);
    
    
    for(int j = 0; j < config->cars_per_team; j++){
        pthread_mutex_lock(&shm->mutex);
        temp = arrayEquipas[idTeam].arrayCarros[j].thread;
        pthread_mutex_unlock(&shm->mutex);
        pthread_join(temp, NULL);
        //printf("EQUIPA %d | Thread a terminar carro %d\n", idTeam, j);
    }
    
}




void team_man_init(){
    

    //printf("Equipa: %d\n", shm->arrayEquipas[idTeam].slot_id);

    
    pthread_mutexattr_init(&attrmutex);
    pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&shm->mutex, &attrmutex);
    
    team *equipa;
    car carros[config->cars_per_team];


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
    
    pthread_mutex_lock(&shm->mutex);
    arrayEquipas[idTeam] = *equipa;

    pthread_mutex_unlock(&shm->mutex);
    
    create_cars();
    
    



}




//main
void team_man(int id, config_struct *_config, shm_struct *_shm, team *array){
    shm = _shm;
    arrayEquipas = array;
    write_log("Team Manager starting");
    printf("Team Manager starting on PID: %d\n", getpid());
    config = _config;
    idTeam = id;
    carsTotal = config->cars_per_team;
    //header = _header;
    sleep(1);
    team_man_init();
    


}

