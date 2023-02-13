#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
#include "memory.h"
#include "semaphore_name.h"

memory_t * main_memory;
int time_turn;
int nb_max_turns = 504; // 2016 turns for 2 weeks of simulation.
struct itimerval set_interval;
struct sigaction action;
int print_flag = 1;
int test = 1;
sem_t *sem_sync[150];

void access_memory() {
    /* Access to the shared memory */
    int shmd = shm_open("/spy_main_memory", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (shmd == -1){
        //fprintf(stderr, "Error with open (memory_t)\n");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shmd, sizeof(memory_t)) == -1){
        //fprintf(stderr, "Error with truncate (memory_t)\n");
        exit(EXIT_FAILURE);
    }

    main_memory = mmap(NULL, sizeof(memory_t), PROT_READ|PROT_WRITE, MAP_SHARED, shmd, 0);
    /* Access garanteed */
}

void update_time_value() {
    main_memory->elapsed_time += time_turn;
    main_memory->minute += 10;
    main_memory->nb_of_turn++;
    if (main_memory->minute == 60) {
        main_memory->minute = 0;
        main_memory->hour++;
        if (main_memory->hour == 24) {
            main_memory->hour = 0;
        }
    }
    main_memory->memory_has_changed = 1;
    //fprintf(stderr, "Times changed: %2d:%02d (%d)\n", main_memory->hour, main_memory->minute, main_memory->nb_of_turn);
}

void send_signal() {
    pid_t ppid = getppid();
    fprintf(stderr, "PID: %d\n", ppid);
    kill(ppid, SIGALRM);
}

void timer_handler(int sig) {
    //fprintf(stderr, "test: %d\n", nb_max_turns);
    int i;
    for (i=0; i<130; i++) {
        sem_wait(sem_sync[i]);
    }
    update_time_value();
    for (i=0; i<130; i++) {
        sem_post(sem_sync[i]);
    }
    send_signal();
    nb_max_turns--;
    if (nb_max_turns == 0) {
        print_flag = 0;
    }
}

void main(int argc, char ** argv) {

    if (argc != 2) {
        //fprintf(stderr, "Missing argument\n");
        exit(EXIT_FAILURE);
    }

    int sec;
    int msec;
    pid_t ppid = getppid();
    for (int i=0; i<130; i++) {
        sem_sync[i] = sem_open(sem_name[i], O_RDWR);
    }

    access_memory();

    sscanf(argv[1], "%d", &time_turn); // must be settled between 100 and 1000 millisecondes)
    sec = time_turn/1000;
    msec = (time_turn%1000)*1000;
    //fprintf(stderr, "time (%d): %ds %dms\n", time_turn, sec, msec/1000);
    set_interval.it_interval.tv_sec = sec;
    set_interval.it_interval.tv_usec = msec;
    set_interval.it_value.tv_sec = sec;
    set_interval.it_value.tv_usec = msec;
    action.sa_handler = &timer_handler;
    sigaction(SIGALRM, &action, NULL);

    setitimer(ITIMER_REAL, &set_interval, NULL);

    while (print_flag) {
        pause();
    }

    //fprintf(stderr, "Pause\n");
    //pause();
    //fprintf(stderr, "Exiting timer\n");
    exit(EXIT_SUCCESS);
}