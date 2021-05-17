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

config_struct *config; //config
shm_struct *shm; //shared memory
team *arrayEquipas;
car *arrayCarros;

pthread_mutexattr_t attrmutex;

pthread_t thread_commands;// Threads
int fd;

//Inicializar
void race_manag_init(){
    
    pthread_mutex_lock(&shm->arrayEquipas_mutex);
    for(int i = 0; i < config->number_of_teams; i++){
        
        arrayEquipas[i].slot_id = i;
        arrayEquipas[i].estadoSlot = LIVRE;
        arrayEquipas[i].numeroCarros = 0;
        arrayEquipas[i].estadoBox = LIVRE;

    }
    
    for(int i = 0; i < config->number_of_teams * config->cars_per_team; i++){
        
        arrayCarros[i].estadoSlot = LIVRE;


    }
    pthread_mutex_unlock(&shm->arrayEquipas_mutex);
    

}

//Terminar
void race_manager_term(){
    write(fd, "EXIT", sizeof("EXIT"));
    close(fd);
    pthread_join(thread_commands, NULL);
}



//THREAD PARA IR VENDO OS COMANDOS DO PIPE(ainda sem utilizar)
void *thread_receives_new_commands(void *arg){

    
    int syntax;
    char buffer[BUF_SIZE];
    write_log("[Race Manager] Starting thread receives_new_commands");
    
    if ((fd = open(PIPE, O_RDWR)) < 0)
        printf("Error when opening pipe\n");

    printf("[RACE MANAGER] Reciving Comands\n");
    
    
    while(1){
        
        read(fd, buffer, BUF_SIZE);
        char copy[strlen(buffer) + 1];
        strcpy(copy, buffer);
        

        
        syntax = verificaSyntax(buffer);
        
        if(syntax == ADDCAR){
            int r;
            r = parse(copy, ADDCAR);
            if(r != ERRO){
                pthread_mutex_lock(&shm->mutex);
                if(arrayCarros[r].visitado == 0){
                    printf("ainda não foi visitado\n");
                }
                while (arrayCarros[r].visitado == 0)
                {
                    pthread_cond_signal(&shm->criaThreadCarro);
                    pthread_cond_wait(&shm->threadCarroCriada,&shm->mutex);
                }
                pthread_mutex_unlock(&shm->mutex);
                
                printf("Comando Aceite\n");
                
            }
            else
                printf("WRONG COMMAND => %s\n", copy);
        }
        else if(syntax == START_RACE){
            if(parse(copy, START_RACE) == 1){
                printf("NEW COMMAND RECEIVED: START RACE\n");
                write_log("NEW COMMAND RECEIVED: START RACE");
            }
            else{
                printf("CANNOT START, NOT ENOUGH TEAMS\n");
                write_log("CANNOT START, NOT ENOUGH TEAMS");
            }
        }
        else if(syntax == EXIT){
            break;
        }
        else{
            printf("WRONG COMMAND => %s\n", copy);
        }
    }
    close(fd);
    printf("[Race Manager] Terminating thread receives_new_commands\n");
    pthread_exit(NULL);
}   


//Verificar Syntax dos comandos do Named Pipe
int verificaSyntax(char *input){
    //ADDCAR TEAM: A, CAR: 20, SPEED: 30, CONSUMPTION: 0.04, RELIABILITY: 95
    //ADDCAR TEAM: B, CAR: 01, SPEED: 32, CONSUMPTION: 0.04, RELIABILITY: 90

    
    int i = 0;
    char copy[strlen(input) + 1];
    strcpy(copy, input);


    //printf("[VERIFY] %s\n", input);
    char *token = strtok(input, " ");
    char *array[10];

    
        

    //fazer split por espaços e colocar num array
    while (token != NULL)
    {
        
        
        array[i] = token;
        array[i][strlen(array[i])] = '\0';
        token = strtok(NULL, " ");
        i++;
    }


    //TODO -> falta verificar se os valores não são strings
    if(strcmp("ADDCAR", array[0]) == 0){
        
        
        if(strcmp("CAR:", array[3]) != 0 || strcmp("SPEED:", array[5]) != 0
        || strcmp("CONSUMPTION:", array[7]) != 0 || strcmp("RELIABILITY:", array[9]) != 0
        ){
            
            return ERRO;
        }
        else if(strcmp(",", array[2]) == 0 || strcmp(",", array[4]) == 0
        || strcmp(",", array[6]) == 0 || strcmp(",", array[8]) == 0
        ){
            return ERRO;
        }
        return ADDCAR;
    }
    else if(strcmp("START", array[0]) == 0){
        array[1][strlen(array[1])-1] = '\0';
        if(strcmp(array[1], "RACE") == 0)
            return START_RACE;
    
    }
    else if(strcmp("EXIT", array[0]) == 0){
        return EXIT;
    }

    //select 

    return ERRO;
}

//-> Verificar se há carros sufecientes para começar a corrida
//-> Adicionar carro à respetiva equipa
int parse(char *input, int type){
    int i;
    char team[BUF_SIZE];
    int carroId, speed, reliability;
    float consumption;
    int carrosTotal = config->number_of_teams * config->cars_per_team;
    
    

    if(type == ADDCAR){
        char *token = strtok(input, " ");
        char *array[10];

        while (token != NULL)
        {
            
            
            array[i] = token;
            array[i][strlen(array[i])] = '\0';
            token = strtok(NULL, " ");
            i++;
        }
        

        array[2][strlen(array[2]) - 1] = '\0';
        strcpy(team, array[2]);
        array[4][strlen(array[4]) - 1] = '\0';
        carroId = atoi(array[4]);
        array[6][strlen(array[6]) - 1] = '\0';
        speed = atoi(array[6]);
        array[8][strlen(array[8]) - 1] = '\0';
        consumption = atof(array[8]);
        array[10][strlen(array[10]) - 1] = '\0';
        reliability = atoi(array[10]);     

        
        int count = 0;
        for(int i = 0; i < carrosTotal;i++){
            if(strcmp(arrayCarros[i].team, team) == 0 && arrayCarros[i].estadoSlot == OCUPADO){
                count++;
            }
        }
        
        if(count >= config->cars_per_team){
            printf("Atingiu o limite de carros\n");
            return ERRO;
        }
        else{
            if(count == 0){
                if(shm->totalEquipasSHM >= config->number_of_teams){
                    return ERRO;
                }
                else{
                    pthread_mutex_lock(&shm->arrayEquipas_mutex);
                    int i = 0;
                    while (arrayEquipas[i].estadoSlot == OCUPADO)
                    {
                        i++;
                    }
                    arrayEquipas[i].estadoSlot = OCUPADO;
                    strcpy(arrayEquipas[i].nome, team);
                    shm->totalEquipasSHM++;
                    pthread_mutex_unlock(&shm->arrayEquipas_mutex);
                }
            }

            pthread_mutex_lock(&shm->arrayCarros_mutex);

            int var = 0;
            int c = 0;
            while (arrayCarros[var].estadoSlot == OCUPADO)
            {
                var++;
            }

            arrayCarros[var].id = carroId;
            arrayCarros[var].speed = speed;
            arrayCarros[var].consumption = consumption;
            arrayCarros[var].reliability = reliability;
            strcpy(arrayCarros[var].team, team);
            arrayCarros[var].estadoSlot = OCUPADO;
            arrayCarros[var].visitado = 0;

            for(int i = 0; i < config->number_of_teams; i++){
                if(strcmp(team, arrayEquipas[i].nome) == 0){
                    c = i;
                    arrayEquipas[i].numeroCarros++;
                }
            }
            arrayCarros[var].idTeam = c;
            shm->totalCarrosSHM++;
            
            pthread_mutex_unlock(&shm->arrayCarros_mutex);
            return var;  
            
        }           
        
    }
    else if(type == START_RACE){
        int count = 0;

        pthread_mutex_lock(&shm->arrayEquipas_mutex);
        for(int i = 0; i < config->number_of_teams; i++){
            if(arrayEquipas[i].estadoSlot == OCUPADO){
                count++;
            }
        }
        pthread_mutex_unlock(&shm->arrayEquipas_mutex);

        if(count == config->number_of_teams){
            return 1;
        }
        else{
            return 0;
        }
    }
    return ERRO;
    
}


void race_manag(config_struct *_config, shm_struct *_shm){
    config = _config;
    shm = _shm;
    arrayEquipas = (team *)(shm + 1);
    arrayCarros = (car *)(arrayEquipas + config->number_of_teams);
    
    write_log("Race Manager starting");
    printf("Race Manager starting on PID: %d e o pai %d\n", getpid(), getppid());
    
    race_manag_init();

    
    
    for(int i= 0; i < config->number_of_teams; i++){
        if(fork() == 0){    
            team_man(i, config, shm);
            exit(0);
            
        }
    }
    

    pthread_create(&thread_commands, NULL, thread_receives_new_commands, NULL);
    
    

    pthread_join(thread_commands, NULL);
    
    
    printf("Sou o race manager %d, vou esperar pelos filhos\n", getpid());
    for(int i= 0; i < config->number_of_teams; i++){
        wait(NULL);
        printf("processo team manager a terminar\n");
    }
        
    printf("##Race Manager TERMINADO\n");
    
   

    
}