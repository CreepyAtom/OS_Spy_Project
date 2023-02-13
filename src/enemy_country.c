/**
 * @author Félix (felix.daune@ecole.ensicaen.fr)
 * @date 08/12/22
 * @brief Ce fichier simule ce que devrait faire l'espion en chef
 */

#include "../include/memory.h"
#include <mqueue.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>




// L'ennemi reçoit tous les messages
int main(int argc, char **argv){
	// Initialisations des mémoires
	int shmd = shm_open("/spy_main_memory", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (shmd == -1){
		printf("Error with open (memory_t)\n");
		exit(EXIT_FAILURE);
	}
	if (ftruncate(shmd, sizeof(memory_t)) == -1){
		printf("Error with truncate (memory_t)\n");
		exit(EXIT_FAILURE);
	}
	memory_t *main_memory = mmap(NULL, sizeof(memory_t), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);


	// Création/ouverture de la mqueue
	mqd_t msg_queue = mq_open("/spy_msg_queue", O_WRONLY|O_CREAT, 0666, NULL);
	if (msg_queue == -1) {
		printf("Error when creating mqueue");
		exit(EXIT_FAILURE);
	}

	// Envoi du message
	mq_send(msg_queue, "Ceci est un autre message.", 50*sizeof(char), 2);

	// Notification au programme principal
	main_memory->memory_has_changed = 1;
}