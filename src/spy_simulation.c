//
// Created by Félix DAUNE on 08/12/22.
//

#include "../include/spy_simulation.h"
#include "../include/monitor.h"
#include "../include/memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "../include/semaphore_name.h"

void init_time_value(memory_t *main_memory) {
    /* Initialisation of the values of for the timer */
    main_memory->minute = 0;
    main_memory->hour = 0;
    main_memory->elapsed_time = 0;
    main_memory->nb_of_turn = 0;
    main_memory->memory_has_changed = 0;
    /* Initialisation done */
}

void sim_handler(int sig) {
	fprintf(stderr, "Handler of simulation called\n");
	pause();
}

int main(int arc, char **argv){

	//// Création de la mémoire partagée principale ////
	int shmd = shm_open("/spy_main_memory", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (shmd == -1){
		//fprintf(stderr, "Error with open (memory_t)\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate(shmd, sizeof(memory_t)) == -1) {
		//fprintf(stderr, "Error with truncate (memory_t)\n");
		exit(EXIT_FAILURE);
	}

	memory_t *main_memory = mmap(NULL, sizeof(memory_t), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);
	struct sigaction action_sim;

	init_time_value(main_memory);

	//// Création de la mémoire partagée pour la map ////
	int shmd2 = shm_open("/spy_map_memory", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (shmd2 == -1){
		//fprintf(stderr, "Error with open (map)\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate(shmd2, sizeof(map_t)) == -1){
		//fprintf(stderr, "Error with truncate (map)\n");
		exit(EXIT_FAILURE);
	}

	map_t *map_memory = mmap(NULL, sizeof(map_t), PROT_READ|PROT_WRITE, MAP_SHARED, shmd2, 0);
	sem_t *sem_sync[150];
	for (int i=0; i<130; i++) {
		sem_sync[i] = sem_open(sem_name[i], O_CREAT|O_RDWR, 0644, 1);
	}

	// Launch monitor
	switch (fork()) {
		case -1:
			//fprintf(stderr, "Erreur : echec du fork()\n");
			exit(EXIT_FAILURE);
		case 0:
			if (-1 == execl("./bin/monitor", "monitor", NULL))
				exit(EXIT_FAILURE);
	}

	// Launch CitizenManager
	switch (fork()) {
		case -1:
			//fprintf(stderr, "Erreur : echec du fork()\n");
			exit(EXIT_FAILURE);
		case 0:
			if (-1 == execl("./bin/citizen_manager", "citizen_manager", NULL)) {
				//fprintf(stderr, "Error: ./bin/CitizenManager cannot be executed\n");
				exit(EXIT_FAILURE);
			}
	}

	// Launch timer
	switch (fork()) {
		case -1:
			//fprintf(stderr, "Erreur : echec du fork()\n");
			exit(EXIT_FAILURE);
		case 0:
			if (-1 == execl("./bin/timer", "timer", "1000", NULL)) {
				//fprintf(stderr, "Error: ./bin/timer cannot be executed\n");
				exit(EXIT_FAILURE);
			}
	}

	action_sim.sa_handler = &sim_handler;
	sigaction(SIGALRM, &action_sim, NULL);

	pause();
	//fprintf(stderr, "Waiting for children\n");
	wait(NULL);

}