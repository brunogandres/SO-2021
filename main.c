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

FILE* log_file;         //LOG FILE
int shmid;              //Shared Memory


config_struct config;   //config struct
shm_struct* shm;


void write_log(char *text){


    char buff[9];
    struct tm* time_info;
    time_t timer;

    time(&timer);
    time_info = localtime(&timer);
    strftime(buff, 26, "%H:%M:%S", time_info);

    fprintf(log_file, "%s ", buff);
    fprintf(log_file, "%s ", text);
    fprintf(log_file, "\n");

    fflush(log_file);
}


// Inicia programa
void init(){

    // Open log file
    log_file = fopen(LOG_FILE, "a");
    if(log_file == NULL)
        printf("Error opening log file\n");
    write_log("Log file opened");

    //Read config file
    read_config();


    // Create pipe


    // Shared memory creation and mapping
    write_log("Creating shared memory");
    printf("Creating shared memory\n");

    if((shmid = shmget(IPC_PRIVATE, sizeof(shm_struct), IPC_CREAT|0700)) == -1) {
        write_log("Error on shared memory creation\n");
        exit(1);
    }
    shm = shmat(shmid, NULL, 0);


    // Create message queue    
}


//termina programa
void terminate(){


    write_log("Terminating program");
    fclose(log_file);
    exit(0);
} 

int main(){
    init();
    write_log("Program starting");

    if(fork() == 0){
        race_sim();
        wait(NULL);
    }
    
    
}