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

pthread_mutexattr_t attrmutex;

pthread_t thread_commands;// Threads
regex_t regex_addcar, regex_startrace;

//Inicializar
void race_manag_init(){
    
    
    for(int i = 0; i < config->number_of_teams; i++){
        shm->arrayEquipas[i].slot_id = i;

    }

    
}

//Terminar
void race_manager_term(){
    pthread_join(thread_commands, NULL);
}
//THREAD PARA IR VENDO OS COMANDOS DO PIPE(ainda sem utilizar)
void *thread_receives_new_commands(void *arg){
    
    int fd = 0;

    char buffer[BUF_SIZE];

    printf("[Race Manager] Starting thread receives_new_commands\n");
    if ((fd = open(PIPE, O_RDWR)) < 0)
        printf("Error when opening pipe\n");

    while(1){
        printf("entrou 1\n");
        read(fd, buffer, BUF_SIZE);
        printf("#####%s", buffer);
        if(strcmp(buffer, "EXIT") == 0){
            break;
        }
    }
    close(fd);

    printf("[Race Manager] Terminating thread receives_new_commands");
    pthread_exit(NULL);
}   


//Verificar Syntax dos comandos do Named Pipe
int verificaSyntax(char *input){
    //ADDCAR TEAM: A, CAR: 20, SPEED: 30, CONSUMPTION: 0.04, RELIABILITY: 95
    //ADDCAR TEAM: B, CAR: 01, SPEED: 32, CONSUMPTION: 0.04, RELIABILITY: 90

    
    int i = 0;
    char copy[strlen(input) + 1];
    strcpy(copy, input);


    printf("[VERIFY] %s\n", input);
    char *token = strtok(input, " ");
    char *array[10];

    //memset(array, 0, sizeof(array));

    
    while (token != NULL)
    {
        
        
        array[i] = token;
        array[i][strlen(array[i])] = '\0';
        token = strtok(NULL, " ");
        i++;
    } 
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
        //else if()
        return ADDCAR;
    }
    if(strcmp("START", array[0]) == 0){
        array[1][strlen(array[1])-1] = '\0';
        if(strcmp(array[1], "RACE") == 0)
            return START_RACE;
    
    }

    //select 

    return ERRO;
}

void race_manag(config_struct *_config, shm_struct *_shm){
    config = _config;
    shm = _shm;
    write_log("Race Manager starting");
    printf("Race Manager starting on PID: %d\n", getpid());
    
    race_manag_init();

    //pthread_create(&thread_commands, NULL, thread_receives_new_commands, NULL);
    
    for(int i= 0; i < config->number_of_teams; i++){
        if(fork() == 0){    
            team_man(i, config, shm);
            exit(0);
            
        }
    }

    int fd = 0;
    int syntax;
    char buffer[BUF_SIZE];

    
    if ((fd = open(PIPE, O_RDWR)) < 0)
        printf("Error when opening pipe\n");

    printf("[RACE MANAGER] Reciving Comands\n");
    while(1){
        read(fd, buffer, BUF_SIZE);
        char copy[strlen(buffer) + 1];
        memset(copy, 0, strlen(buffer) + 1);
        strcpy(copy, buffer);
        
        syntax = verificaSyntax(buffer);
        
        if(syntax == ADDCAR){
            printf("Comando Aceite\n");
            adicionaCarro(buffer);
        }
        else if(syntax == START_RACE){
            printf("Comando Aceite\n");
        }
        else{
            printf("WRONG COMMAND => %s\n", copy);
        }
    }
    close(fd);

    
    printf("Sou o race manager %d, vou esperar pelos filhos\n", getpid());
    for(int i= 0; i < config->number_of_teams+1; i++){wait(NULL);
        printf("processo team manager a terminar\n");
    }
    
    printf("##Race Manager TERMINADO\n");
    
   

    
}