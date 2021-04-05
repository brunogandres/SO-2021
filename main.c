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
int shmid, teams_shmid;              //Shared Memory


config_struct* config;   //config struct
shm_struct* shm;



Node linked_list_create(){
    Node aux = malloc(sizeof(tnode));
    if(aux == NULL){
        printf("Error on malloc\n");
        exit(-1);
    }
    aux->cargo = NULL;
    aux->next = aux->previous = NULL;

    return aux;
}

Node linked_list_insert(Node header, void* cargo){
    Node temp, new = malloc(sizeof(tnode));
    if(new == NULL){
        printf("Error on malloc\n");
        exit(-1);
    }
    new->cargo = cargo;
    
    //vai ter de levar mutex
    // Finds where to insert new node
    for(temp = header; temp->next != NULL; temp = temp->next);

    new->next = temp->next;
    new->previous = temp;
    if(temp->next != NULL)
        temp->next->previous = new;
    temp->next = new;

    return new;
}

void linked_list_remove(Node header, Node node){

    if(node->next != NULL)
        node->next->previous = node->previous;

    if(node->previous != NULL)
        node->previous->next = node->next;
    free(node);
}



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

    config = malloc(sizeof(config_struct));
    if(config == NULL){
        printf("Error on malloc\n");
        exit(-1);
    }

    read_config();
    


    // Create pipe


    // Shared memory creation and mapping
    write_log("Creating shared memory");
    printf("Creating shared memory\n");

    if((shmid = shmget(IPC_PRIVATE, sizeof(shm_struct), IPC_CREAT|0700)) == -1) {
        write_log("Error on shared memory creation\n");
        exit(1);
    }
    if ((shm = (shm_struct *) shmat(shmid, NULL, 0)) == (shm_struct*)-1) {
		perror("Shmat error!");
		exit(1);
	}

    
    if((teams_shmid = shmget(IPC_PRIVATE, sizeof(team), IPC_CREAT|0700)) == -1){
        exit(1);
    }
    shm->arrayEquipas = shmat(teams_shmid, NULL, 0);
    
    
    // Create message queue    
}


//termina programa
void terminate(){


    write_log("Terminating program");
    race_sim_terminate();

    shmdt(shm->arrayEquipas);
    shmctl(teams_shmid, IPC_RMID, NULL);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL); // Destroy main shared memory

    write_log("Shared memory destroyed");
    write_log("Terminating program");
    
    free(config);
    fclose(log_file);
    exit(0);
} 



int main(){
    init();
    write_log("Program starting");

    if(fork() == 0){
        race_sim(config, shm);
        wait(NULL);
    }
    
    
}