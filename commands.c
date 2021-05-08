/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador de Corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPE "input_pipe"

int fd;

int main(){
    char arr[100];
    fd = open(PIPE, 0666);
    
    while(1){
        //printf("Insere os comandos: \n");
        fgets(arr, sizeof(arr), stdin);
        if(strlen(arr) > 1) {
            arr[strlen(arr)] = '\0';
            write (fd, arr, strlen (arr) + 1);
        }
    }

}