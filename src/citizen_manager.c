//
// Created by Félix on 20/12/22.
//

// @TODO ne pas oublier "-lptherad" pour l'édition des liens

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#include "common.h"
#include "character.h"
#include "semaphore_name.h"
#include "memory.h"

int id_incr = 1;
int firms[8][3]; // Donne l'accès au type d'une entreprise et ses coordonnées
int type_firms[8][2]; // Donne la liste des entreprises (coordonnées) selon leur type
int homes[11][3];
int index_secret_box;
sem_t sem_data;
map_t *map_memory;

void access_map() {
	// Access to the shared map //
    int shmd = shm_open("/spy_map_memory", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (shmd == -1){
        fprintf(stderr, "Error with open (map)\n");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shmd, sizeof(map_t)) == -1){
        fprintf(stderr, "Error with truncate (map)\n");
        exit(EXIT_FAILURE);
    }

    map_memory = mmap(NULL, sizeof(map_t), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);
    // Access granted //
}

int pop_i_max(int *tab, int len) {
	int i;
	int i_max = 0;
	for (i=0; i<len; i++) {
		if (tab[i] > tab[i_max]) {
			i_max = i;
		}
	}
	tab[i_max] = 0;
	return i_max;
}

void update_type_firms(int i_firm, int index, int type) {
	firms[index][2] = type; // L'entreprise est initialisée par ce type
	type_firms[i_firm][0] = firms[index][0]; // Les coordonnées de l'entreprise sont enregistrées (ordre de type)
	type_firms[i_firm][1] = firms[index][1];
}

void set_pos(int *tab, int x, int y) {
	tab[0] = x;
	tab[1] = y;
}

void init_firms() {
	int i;
	int i_max;
	int t_rand[8];
	set_pos(firms[0], 0, 1);
	set_pos(firms[1], 0, 3);
	set_pos(firms[2], 0, 6);
	set_pos(firms[3], 3, 4);
	set_pos(firms[4], 4, 2);
	set_pos(firms[5], 5, 2);
	set_pos(firms[6], 5, 6);
	set_pos(firms[7], 6, 1);
	for (i=0; i<8; i++) {
		t_rand[i] = 1 + (int) rand()%100;
	}
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(0, i_max, 3); // VERY SMALL
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(1, i_max, 3); // VERY SMALL
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(2, i_max, 3); // SMALL
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(3, i_max, 4); // SMALL
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(4, i_max, 4); // SMALL
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(5, i_max, 5); // MEDIUM
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(6, i_max, 5); // MEDIUM
	i_max = pop_i_max(t_rand, 8);
	update_type_firms(7, i_max, 6); // BIG
}

void init_homes() {
	int i;
	set_pos(homes[0], 1, 0);
	set_pos(homes[1], 1, 5);
	set_pos(homes[2], 2, 1);
	set_pos(homes[3], 2, 3);
	set_pos(homes[4], 2, 6);
	set_pos(homes[5], 4, 1);
	set_pos(homes[6], 4, 6);
	set_pos(homes[7], 5, 4);
	set_pos(homes[8], 6, 0);
	set_pos(homes[9], 6, 4);
	set_pos(homes[10], 6, 6);
	for (i=0; i<11; i++) {
		homes[i][2] = 0;
	}
	index_secret_box = (int) rand()%11;
}

void set_firm_data_to_citizen(character_t *citizen, int type, int x, int y) {
	citizen->type_firm = type;
	citizen->x_firm = x;
	citizen->y_firm = y;
}

void get_firm(character_t *citizen) {
	int id = citizen->citizen_id % 1000;
	if (id < 11) { set_firm_data_to_citizen(citizen, 1, 3, 3); }
	else if (id < 14) { set_firm_data_to_citizen(citizen, 2, 1, 2); }
	else if (id < 17) { set_firm_data_to_citizen(citizen, 2, 4, 3); }
	else if (id < 22) { set_firm_data_to_citizen(citizen, 3, type_firms[0][0], type_firms[0][1]); }
	else if (id < 27) { set_firm_data_to_citizen(citizen, 3, type_firms[1][0], type_firms[1][1]); }
	else if (id < 32) { set_firm_data_to_citizen(citizen, 3, type_firms[2][0], type_firms[2][1]); }
	else if (id < 43) { set_firm_data_to_citizen(citizen, 4, type_firms[3][0], type_firms[3][1]); }
	else if (id < 54) { set_firm_data_to_citizen(citizen, 4, type_firms[4][0], type_firms[4][1]); }
	else if (id < 75) { set_firm_data_to_citizen(citizen, 5, type_firms[5][0], type_firms[5][1]); }
	else if (id < 96) { set_firm_data_to_citizen(citizen, 5, type_firms[6][0], type_firms[6][1]); }
	else { set_firm_data_to_citizen(citizen, 6, type_firms[7][0], type_firms[7][1]); }
}

void get_home(character_t *citizen) {
	int index = (int) rand()%11;
	sem_wait(&sem_data);
	while ((index == index_secret_box) || homes[index][2] == 15) {
		index = (int) rand()%11;
	}
	sem_post(&sem_data);
	homes[index][2]++;
	citizen->x_home = homes[index][0];
	citizen->y_home = homes[index][1];
}

character_t * create_a_citizen(int id) {
	state_t *resting_at_home = new_state(1, rest_at_home);
	//fprintf(stderr, "ID: %d\n", resting_at_home->id);
    state_t *going_to_company = new_state(2, go_to_company);
    state_t *working = new_state(3, work);
    state_t *going_to_supermarket = new_state(4, go_to_supermarket);
    state_t *doing_some_shopping = new_state(5, do_some_shopping);
    state_t *going_back_home = new_state(6, go_back_home);

	character_t *citizen = new_character(resting_at_home,
										going_to_company,
										working,
										going_back_home,
										going_to_supermarket,
										doing_some_shopping);
	//fprintf(stderr, "ID: %d\n", citizen->resting_at_home->id);
	//fprintf(stderr, "ID: %d\n", citizen->current_state->id);
	citizen->citizen_id = id;
	get_firm(citizen);
	get_home(citizen);
	return citizen;
}

void manage_citizen(character_t * citizen) {
	int i = 0;
	citizen->begin(citizen);
	while (i < 2016) {
		sem_t *sem_sync = sem_open(sem_name[citizen->citizen_id], O_RDWR);
		sem_wait(sem_sync);
		citizen->step(citizen);
		sem_post(sem_sync);
		i++;
	}
	citizen->end(citizen);
}

void *init_citizen() {
	int id = id_incr++;
	character_t * citizen = create_a_citizen(id);
	fprintf(stderr, "Citizen created (%04ld) (ID:%d) (FIRM:(%d, %d) %d) (HOME:(%d, %d))\n", pthread_self()%10000, citizen->citizen_id, citizen->x_firm, citizen->y_firm, citizen->type_firm, citizen->x_home, citizen->y_home);
	manage_citizen(citizen);
	return NULL;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	int sum = 0;
	pthread_t tid[NB_OF_NORMAL_CITIZEN];

	init_firms();
	init_homes();
	access_map();

	sem_init(&sem_data, 0, 1); // Used only to initialise the data between the threads

	// Créer les citoyens avec NB_OF_NORMAL_CITIZEN
	for (unsigned int i = 0; i < NB_OF_NORMAL_CITIZEN; ++i) {
		pthread_create(tid+i, NULL, init_citizen, NULL);
	}

	for (unsigned int i = 0; i < NB_OF_NORMAL_CITIZEN; ++i) {
		//fprintf(stderr, "Joining...\n");
		pthread_join(tid[i], NULL);
		//fprintf(stderr, "Joined (%d/%d)\n", i+1, size);
	}

	for (int i=0; i<11; i++) {
		fprintf(stderr, "HOME (%d, %d): %d citizens\n", homes[i][0], homes[i][1], homes[i][2]);
		sum += homes[i][2];
	}
	fprintf(stderr, "Citizens: %d\n", sum);

	sem_destroy(&sem_data);

}