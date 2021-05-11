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

pthread_mutexattr_t attrmutex;

pthread_t thread_commands;// Threads
int fd;

//Inicializar
void race_manag_init(){
    
    
    for(int i = 0; i < config->number_of_teams; i++){
        
        arrayEquipas[i].slot_id = i;

    }
    

    
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
            printf("Comando Aceite\n");
            
            parse(copy, ADDCAR);
            
            //adicionaCarro(buffer);
        }
        else if(syntax == START_RACE){
            printf("Comando Aceite\n");
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


    printf("[VERIFY] %s\n", input);
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

int parse(char *input, int type){
    int i;
    car *carro;
    char team[BUF_SIZE];
    int carroId, speed, reliability;
    float consumption;
    


    if(type == ADDCAR){
        //fgets(input, 100, stdin);
        char *token = strtok(input, " ");
        char *array[10];

        while (token != NULL)
        {
            
            
            array[i] = token;
            array[i][strlen(array[i])] = '\0';
            token = strtok(NULL, " ");
            i++;
        }
        carro = malloc(sizeof(car));
        if(carro == NULL){
            printf("Error on malloc\n");
            exit(-1);
        }

        array[2][strlen(array[2]) - 1] = '\0';
        strcpy(team, array[2]);
        array[4][strlen(array[4]) - 1] = '\0';
        carroId = atoi(array[4]);
        array[6][strlen(array[6]) - 1] = '\0';
        speed = atoi(array[6]);

        
    }
    return 0;
    
}


void race_manag(config_struct *_config, shm_struct *_shm, team *array){
    config = _config;
    shm = _shm;
    arrayEquipas = array;
    
    write_log("Race Manager starting");
    printf("Race Manager starting on PID: %d e o pai %d\n", getpid(), getppid());
    
    race_manag_init();

    pthread_create(&thread_commands, NULL, thread_receives_new_commands, NULL);
    /*
    for(int i= 0; i < config->number_of_teams; i++){
        if(fork() == 0){    
            team_man(i, config, shm, arrayEquipas);
            exit(0);
            
        }
    }*/
    

    pthread_join(thread_commands, NULL);
    
    
    printf("Sou o race manager %d, vou esperar pelos filhos\n", getpid());
    for(int i= 0; i < config->number_of_teams+1; i++){
        wait(NULL);
        printf("processo team manager a terminar\n");
    }
        
    printf("##Race Manager TERMINADO\n");
    
   

    
}