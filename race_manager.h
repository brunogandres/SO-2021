/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */

void race_manag(config_struct *_config, shm_struct *_shm);
void race_manag_init();
void *thread_receives_new_commands(void *arg);
void race_manager_term();
int verificaSyntax(char *input);