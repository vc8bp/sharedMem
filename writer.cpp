#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unordered_map>
#include <mutex> // Include the mutex header

#include "struct.h"

std::unordered_map<int, SharedData*> myMap;
std::mutex mapMutex; // Mutex to synchronize access to the map

void append(int token) {
    SharedData* sharedData;
    int data = std::rand() % 1000;
    int price = std::rand() % 1000;

    {
        std::lock_guard<std::mutex> lock(mapMutex); // Lock the map during the critical section

        if (myMap.find(token) == myMap.end()) {
            std::cout << "Token Not Found" << std::endl;

            key_t key = ftok("/home/finrise/project/sharedMem/mem", token);

            if (key == -1) {
                perror("ftok");
            }

            if(token == 1) std::cout<< " KEY : " << token << " Value : " << price << std::endl ;

            int shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);

            if (shmid == -1) {
                std::cerr << "Error creating or getting shared memory segment for token " << token << std::endl;
                return;
            }

            sharedData = (SharedData*)shmat(shmid, nullptr, 0);

            if (sharedData == (SharedData*)-1) {
                std::cerr << "Error attaching shared memory for token " << token << std::endl;
                return;
            }

            myMap[token] = sharedData;
        } else {
            if(token == 1) std::cout<< " KEY : " << token << " Value : " << price << std::endl ;
            sharedData = myMap[token];
        }
    } // Unlock the map outside the critical section

    sharedData->token = token;
    sharedData->data1 = data;
    sharedData->price = price;
}

int main() {
    std::srand(std::time(0));  // Seed for random number generation

    while (true) {
        int randomToken = std::rand() % 10 + 1;
        append(randomToken);
    }

    // Cleanup before exiting
    for (auto& entry : myMap) {
        shmdt(entry.second);
        // You may consider removing the shared memory segment using shmctl here
    }

    return 0;
}
