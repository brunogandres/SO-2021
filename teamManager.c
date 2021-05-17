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
car *arrayCarros;

int carsTotal;
int idTeam;
pthread_t verificaSHM;






void *thread_sim_car(void *carro){
    return 0;
}


void *create_cars(void *arg){
    
    int r[carsTotal];
    int thread;

    while (1)
    {
        pthread_mutex_lock(&shm->mutex);
        //shm->totalCarrosSHM < carsTotal
        while (1)
        {
            pthread_cond_wait(&shm->criaThreadCarro,&shm->mutex);
        }
        
        
        //pthread_mutex_lock(&shm->arrayCarros_mutex);
        for(int i = 0; i < carsTotal; i++){
            //printf("entoru\n");
            if(arrayCarros[i].idTeam == idTeam){
                
                if(arrayCarros[i].visitado == 0){
                    r[i] = i;
                    thread = pthread_create(&arrayCarros[i].thread, NULL, thread_sim_car, &r[i]);
                    if(thread == 0){
                        printf("[%d] Thread Carro crida com o id %d\n", arrayCarros[i].idTeam, arrayCarros[i].id);

                        arrayCarros[i].visitado = 1;
                        
                    }
                }
            }
        }
        //pthread_mutex_unlock(&shm->arrayCarros_mutex);

        pthread_cond_broadcast(&shm->threadCarroCriada);
        pthread_mutex_unlock(&shm->mutex);
        
    }
    return NULL;
    

    
}




void team_man_init(){
    
    pthread_create(&verificaSHM, NULL, create_cars, NULL);
    
    pthread_exit(NULL);
}




//main
void team_man(int id, config_struct *_config, shm_struct *_shm){
    shm = _shm;
    arrayEquipas = (team *)(shm + 1);
    arrayCarros = (car *)(arrayEquipas + config->number_of_teams);
    write_log("Team Manager starting");
    printf("Team Manager starting on PID: %d\n", getpid());
    config = _config;
    idTeam = id;
    carsTotal = config->cars_per_team * config->number_of_teams;

    //header = _header;
    sleep(1);
    team_man_init();

    


}

