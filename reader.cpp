#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <string>
#include <cerrno>  // Include errno header
#include <cstring> // Include cstring header for strerror

#include "struct.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <token>" << std::endl;
        return 1;
    }

    int token = atoi(argv[1]);

    key_t key = ftok("Randommm", token);

    std::string keyString = "SharedMem" + std::to_string(token);

    if (key == -1) {
        std::cerr << "Error in ftok: " << strerror(errno) << std::endl;
        return 1; // Handle the error appropriately
    }

    std::cout<< key << std::endl;
    int shmid = shmget(key, sizeof(SharedData), 0666);

    while (true) {
        // Attach the shared memory segment
        SharedData* sharedData = (SharedData*)shmat(shmid, NULL, 0);
        if (sharedData == (SharedData*)-1) {
            std::cerr << "Error attaching shared memory." << std::endl;
            return 1;
        }

        // Read data based on token
        std::cout << "Token: " << sharedData->token << std::endl;
        std::cout << "Data1: " << sharedData->data1 << std::endl;
        std::cout << "Price: " << sharedData->price << std::endl;

        // Detach the shared memory segment
        shmdt(sharedData);

        // Sleep for a while before the next iteration
        usleep(100000);  // 500 milliseconds, adjust as needed
    }

    return 0;
}
