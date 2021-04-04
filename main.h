/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <regex.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "race_simulator.h"
#include "teamManager.h"
#include "malfunction_manager.h"
#include "race_manager.h"


#define LOG_FILE "log.txt"

#define CONFIG_FILE "config.txt"









Node linked_list_create();
Node linked_list_insert(Node header, void* cargo);
void linked_list_remove(Node header, Node node);

void write_log(char *text);
void init();
void terminate();



