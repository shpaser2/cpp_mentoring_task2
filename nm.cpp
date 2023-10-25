#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include <thread>
#include <chrono>

#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/exceptions.hpp>

#include <memory>

#include <mutex>
#include <array>
#include <atomic>
#include <condition_variable>
#include <fstream>

namespace bip = boost::interprocess;
const std::string mutex_r_name = "shared_mutex_a";
const std::string mutex_w_name = "shared_mutex_b";

const size_t BUFFER_SIZE = 4096;

struct Buffer {
    std::mutex mtx;
    std::condition_variable cv;
    std::array<char, 4096> data;
    std::streamsize bytesRead {0};
    std::atomic<bool> readyForReading {false};
    std::atomic<bool> readyForWriting {true};
};

// // global variables
// Buffer buffer1, buffer2;
// std::atomic<bool> isReadingFinished {false};
struct shared_buffer{
    std::unique_ptr<bip::named_mutex> mtx;
    std::unique_ptr<bip::named_condition> cv;
    bip::managed_shared_memory managed_shm; //{open_or_create, "shm", 1024};
    std::array<char,BUFFER_SIZE> *array;

/*ATTENTION! "Example 33.11. Atomic access on a managed shared memory"

looks like I can create any variable right inside shm and get access to it 
    by shm and variable name. And atomic access too.
https://theboostcpplibraries.com/boost.interprocess-managed-shared-memory
*/

    //std::streamsize bytesRead {0};
    // std::atomic<bool> readyForReading {false};
    // std::atomic<bool> readyForWriting {true};
};

struct common_variables{

    std::atomic<bool> isReadingFinished {false};
}

shared_buffer make_shared_buffer(const std::string& name) {
    shared_buffer b{};
    printf("1111111111111111111\n");
    b.mtx = std::make_unique<bip::named_mutex>(
            bip::open_or_create, ("shared_mutex_" + name).c_str()
    );
    printf("2222222222222222222222\n");
    b.cv = std::make_unique<bip::named_condition>(
            bip::open_or_create, ("shared_cv_" + name).c_str()
    );
    printf("333333333333333333333\n");
    b.managed_shm = bip::managed_shared_memory(
            bip::open_or_create, ("shared_memory_" + name).c_str(), 
            2048
        );
    printf("4444444444444444444444\n");
    b.array = b.managed_shm.find_or_construct<std::array<char,20>>("buffer")();
    printf("5555555555555555555555555\n");
    return std::move(b);
}

// Debug function to find out whether the mutex is still held or not.
void check_mutex_lock(const bip::scoped_lock<bip::named_mutex>& mtx_lock) {
    if (mtx_lock.owns()) {
        std::cout << "Lock owns the mutex" << std::endl;
    } else {
        std::cout << "Lock does not own the mutex" << std::endl;
    }
}

int main()
{
    shared_buffer b_a = make_shared_buffer("a");
    std::cout << "After making a \n";
    std::unique_ptr<bip::named_mutex> mutex_r_ptr;
    std::unique_ptr<bip::named_mutex> mutex_w_ptr;
    bool isReader = false;
    bool isWriter = false;

    // Acquire shared resources for the process.
    try {
        mutex_r_ptr.reset(new bip::named_mutex(
            bip::open_or_create, mutex_r_name.c_str()));
        mutex_w_ptr.reset(new bip::named_mutex(
            bip::open_or_create, mutex_w_name.c_str()));
    } catch (const bip::interprocess_exception &ex) {
        std::cout << "Error: " << ex.what() << std::endl;
        return 1;
    }

    // Select a role for the process.
    bip::scoped_lock<bip::named_mutex> lock_r(*mutex_r_ptr,
                                            bip::defer_lock);
    bip::scoped_lock<bip::named_mutex> lock_w(*mutex_w_ptr,
                                             bip::defer_lock);
    if (lock_r.try_lock()) {
        isReader = true;
        printf("R took\n");
    } else if (lock_w.try_lock()) {
        isWriter = true;
        printf("W took\n");
    } else {
        printf("N exited\n");
        return 0;
    }

    // Do the job according to the process's role.
    if (isWriter) {
        printf("W check\n");
        check_mutex_lock(lock_w);
    }
    if (isReader) {
        printf("R check\n");
        check_mutex_lock(lock_r);
    }

    // Wait for 50ms
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return 0;
}
