/*
 *  Licenciatura em Engenharia Informática
 *
 *  Sistemas Operativos
 *  Projeto - Simulador	de	corridas
 *
 *  2018295305 - Bruno Damião Areias Gandres
 *
 */



void team_man(int id, config_struct *_config, shm_struct *_shm, team *array);









void *thread_sim_car(void *carro);
void create_Cars();

void team_man_init();
void team_man_terminate();