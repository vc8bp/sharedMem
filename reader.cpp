#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cerrno>
#include <cstring>

#include "struct.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <token>" << std::endl;
        return 1;
    }

    int token = atoi(argv[1]);

    key_t key = ftok("/home/finrise/project/sharedMem/mem", token);
    
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int shmid = shmget(key, sizeof(SharedData), 0666);

    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    struct SharedData *sharedData = (struct SharedData *)shmat(shmid, NULL, 0);
    while (true) {

        

        // Read data based on token
        std::cout << "Price: " << sharedData->price << std::endl;

        // Detach the shared memory segment


        // Sleep for a while before the next iteration
    }

    if (shmdt(sharedData) == -1) {
        perror("shmdt");
        return 1;
    }

    return 0;
}
