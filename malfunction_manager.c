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
config_struct *config;

void malfunc_manager(config_struct *_config){
    config = _config;
    write_log("Malfunction Manager starting");
    printf("Malfunction Manager starting on PID: %d AND FATHER IS %d\n", getpid(), getppid());

    wait(NULL);
    printf("##MalFunction TERMINADO\n");
}
