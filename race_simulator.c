#include "race_simulator.h"


FILE* log_file;         //LOG FILE
int shmid, teams_shmid;              //Shared Memory


config_struct* config;   //config struct
shm_struct* shm;
int pid[2]; // saves the pid of child processes

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
    write_log("Creating pipe");
    unlink(PIPE);
    if(mkfifo(PIPE, O_CREAT|O_EXCL|0600) < 0) {
        write_log("Error on FIFO creation");
        exit(1);
    }


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

    shm->arrayEquipas = malloc(config->number_of_teams * sizeof(team));


    
    
    // Create message queue    
}
void terminate(){
    write_log("Terminating program");
    printf("TERMINATE\n");
    signal(SIGINT, SIG_IGN);
    
    /*
    pthread_t temp;
    //team_man_terminate();

    for(int i = 0; i < config->number_of_teams; i++){
        for(int j = 0; j < config->cars_per_team; j++){
            pthread_mutex_lock(&shm->mutex);
            temp = shm->arrayEquipas[i].arrayCarros[j].thread;
            pthread_mutex_unlock(&shm->mutex);
            pthread_join(temp, NULL);
            printf("EQUIPA %d | Thread a terminar carro %d\n", i, j);
        }
    }
    */
    
    //race_sim_terminate();
    race_manager_term();
    wait(NULL);

    shmdt(shm->arrayEquipas);
    shmctl(teams_shmid, IPC_RMID, NULL);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL); // Destroy main shared memory



    write_log("Shared memory destroyed");
    write_log("Terminating program");
    //close(PIPE);
    free(config);
    fclose(log_file);
    exit(0);
} 
void estatisticas(){
    
}
int main(){    

    signal(SIGINT, terminate);
    signal(SIGTSTP, estatisticas);
    
    init();
    
    
    /*
    if(fork() == 0){
        race_manag(config,shm);
    }else{
        if(fork() == 0){
            malfunc_manager(config);
        }
        else{
            printf("Race Simulator starting [%d]\n", getpid());
            for(int i = 0; i < 2; i++) wait(NULL);
            printf("##Race simulator a terminar\n");
            terminate();
          
        }
    }
    */
    
    if((pid[0]=fork())==0){
        signal(SIGINT, terminate);
	    race_manag(config,shm);
	    exit(0);
	}
    if((pid[1]=fork())==0){
        //signal(SIGINT, terminate);
	    malfunc_manager(config);
	    exit(0);
	}
    

    printf("Race Simulator starting [%d]\n", getpid());
    for(int i = 0; i < 2; i++) wait(NULL);
    printf("##Race Simulator TERMINADO\n");
    terminate();

    exit(0);    

    

    
    
    
    
    
    
    
    
}