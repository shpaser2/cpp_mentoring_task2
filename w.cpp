#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

// #include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/exceptions.hpp>

namespace bip = boost::interprocess;
const std::string mutexName = "shared_mutex";

int main()
{
    bip::named_mutex mutex(bip::open_or_create, mutexName.c_str());
    bool isReader = false;
    bool isWriter = false;

    try {
        bip::scoped_lock<bip::named_mutex> lock(mutex, bip::defer_lock);
        if (lock.try_lock()) {
            isReader = true;
        } else {
            lock.unlock();
            isWriter = true;
        }
    } catch (const bip::interprocess_exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    if (isWriter) {
        printf("W");
    }
    if (isReader) {
        printf("R");
    }


//w/r
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);  
    // shmget returns an identifier in shmid
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,(void*)0,0);
  
//w 
    std::cout<<"Write Data : ";
    fgets(str, 1024, stdin); 
    // Use fgets instead of gets
  
    printf("Data written in memory: %s\n",str);

//close      
    //detach from shared memory 
    shmdt(str);
    printf("w.cpp is ended\n");
  
    return 0;
}