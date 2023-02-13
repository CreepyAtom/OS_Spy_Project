#include <memory.h>

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