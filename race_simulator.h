/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */

typedef struct
{
    int time_unit;
    int lap_distance; //metros
    int number_of_laps;
    int number_of_teams;
    int T_avaria;
    int T_Box_min;
    int T_Box_max;
    int fuel_tank;
}config_struct;



typedef struct 
{
    //stats_struct stats; --> mais para a frente

    //semaphores

    //condition variables

    //mutexes

    //lista ligada com as boxs
    int i;
    

}shm_struct;







void read_config();
void race_sim_init();
void race_sim_terminate();
void race_sim();