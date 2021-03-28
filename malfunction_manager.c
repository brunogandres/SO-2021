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

void malfunc_manager(){
    write_log("Malfunction Manager starting");
    printf("Malfunction Manager starting on PID: %d AND FATHER IS %d\n", getpid(), getppid());
}
