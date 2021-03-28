/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */


void team_man();

#define MAX_LEN_NAME 100



// Cars structs
typedef struct{

    pthread_t thread;


    char name[MAX_LEN_NAME];
    int estado; //1-Corrida 2-Segurança 3-Box 4-Desistencia 5-Terminado
    
    int speed;
    int consumption;



} car;




void create_Cars();