/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */




#define MAX_LEN_NAME 100

typedef struct
{
    int time_unit;
    int lap_distance; //metros
    int number_of_laps;
    int number_of_teams;
    int cars_per_team;
    int T_avaria;
    int T_Box_min;
    int T_Box_max;
    int fuel_tank;
}config_struct;




// Car struct
typedef struct{

    pthread_t thread;

    int slot_id, id;

    char name[MAX_LEN_NAME];
    int estado; //1-Corrida 2-Segurança 3-Box 4-Desistencia 5-Terminado
    
    int speed;
    int consumption;
} car;

typedef struct
{
    
    int totalAvarias;
    int totalAbastecimentos;
    int carrosEmPista;
    car* lastPosition;

}stats_struct;

// Cars structs

typedef struct
{
    int slot_id;        //slot shm
    int estadoBox; //1-ocupado 0-livre
    car* arrayCarros;
    int *ids;
    int id;
    char nome[10];
}team;


typedef struct 
{
    stats_struct stats; //--> mais para a frente


    pthread_mutex_t mutex;

    //array equipas
    team *arrayEquipas;

    //array de carros em pista
    car* cars;

}shm_struct;

typedef struct Node* Node;
typedef struct Node{
    void* cargo;
    Node previous, next;
} tnode;











void read_config();
void race_sim_init();
void race_sim_terminate();
void race_sim(config_struct *_config, shm_struct *_shm);
