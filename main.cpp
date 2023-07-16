#include <thread>
#include "rw.h"
#include <iostream>

// #include <fstream>
// #include <mutex>
// #include <array>
// #include <atomic>
// #include <condition_variable>

// #ifdef DEBUG
// #include <chrono>
// #endif

// struct Buffer {
//     std::mutex mtx;
//     std::condition_variable cv;
//     std::array<char, 4096> data;
//     std::streamsize bytesRead {0};
//     std::atomic<bool> readyForReading {false};
//     std::atomic<bool> readyForWriting {true};
// };

// //global variables
// Buffer buffer1, buffer2;
// std::atomic<bool> isReadingFinished {false};

// bool handleReadToBuffer(Buffer& buffer, std::ifstream& inputFile);
// void handleWriteFromBuffer(Buffer& buffer, std::ofstream& outputFile);
// bool readyToStopWriting(Buffer& buffer1, Buffer& buffer2);


// void readFromFile(const std::string& source) {
//     std::cerr << "r1\n";
//     std::ifstream inputFile(source, std::ios::binary);  
//     if (!inputFile) {
//         std::cerr << "r1exit\n";
//         std::cerr << "Unable to open source file.\n";
//         return;
//     }
//     std::cerr << inputFile.good() << std::endl;
//     std::cerr << "r2\n";

//     while (true) {

// #ifdef DEBUG
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
// #endif
//         std::cerr << "r3\n";
//         if (!handleReadToBuffer(buffer1, inputFile) || 
//             !handleReadToBuffer(buffer2, inputFile)) {
//             break;
//         }
//     }
//     std::cerr << "r13\n";
// }


// void writeToFile(const std::string& destination) {
//     std::cerr << "w1\n";

//     std::ofstream outputFile(destination, std::ios::binary);  
//     if (!outputFile) {
//         std::cerr << "Unable to open destination file.\n";
//         std::cerr << "w2\n";
//         return;
//     }

//     std::cerr << "w3\n";

//     while (true) {

// #ifdef DEBUG
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
// #endif
//         std::cerr << "w4\n";
//         handleWriteFromBuffer(buffer1, outputFile);
//         handleWriteFromBuffer(buffer2, outputFile);
//         if (readyToStopWriting(buffer1, buffer2)) {
//             break;
//         }
//     }
//     std::cerr << "w10\n";
// }

int main(int argc, char *argv[]) {
    std::cerr << "m1\n";
    if (argc != 3) {
        std::cerr << "m2\n";
        std::cerr << "Usage: " << argv[0] << " <source> <destination>\n";
        return 1;
    }
    std::cerr << "m3\n";

    std::string source = argv[1];
    std::string destination = argv[2];
    std::cerr << "m4\n";

    std::thread reader(readFromFile, source);
    std::thread writer(writeToFile, destination);
    std::cerr << "m5\n";

    reader.join();
    std::cerr << "m6\n";
    writer.join();
    std::cerr << "m7\n";

    return 0;
}


// bool handleReadToBuffer(Buffer& buffer, std::ifstream& inputFile) {
//     std::unique_lock<std::mutex> lock(buffer.mtx); 
//     buffer.cv.wait(lock, [&buffer]{return buffer.readyForWriting.load();});
    
//     std::cerr << "r5a\n";
//     inputFile.read(buffer.data.data(), buffer.data.size());
//     buffer.bytesRead = inputFile.gcount();
//     buffer.readyForWriting = false;
//     buffer.readyForReading = true;
//     std::cerr << "r6a\n";

//     if (buffer.bytesRead == 0) {   //error or nothing to read
//         std::cerr << "r9a\n";
//         isReadingFinished = true;
//         return false;
//     }
//     std::cerr << "r10a\n";

//     lock.unlock();
//     buffer.cv.notify_one();
//     return true;
// }

// void handleWriteFromBuffer(Buffer& buffer, std::ofstream& outputFile) {
//     std::unique_lock<std::mutex> lock(buffer.mtx);
//     buffer.cv.wait(lock, [&buffer]{return buffer.readyForReading.load();});
        
//     std::cerr << "w6a\n";
//     outputFile.write(buffer.data.data(), buffer.bytesRead);
//     std::cerr << "w7a\n";
//     buffer.bytesRead = 0;
//     buffer.readyForReading = false;
//     buffer.readyForWriting = true;

//     std::cerr << "w9a\n";
//     lock.unlock();
//     buffer.cv.notify_one();
// }

// bool readyToStopWriting(Buffer& buffer1, Buffer& buffer2) {
//     static bool buffer1isEmpty;
//     static bool buffer2isEmpty;

//     {
//         std::unique_lock lock(buffer1.mtx);
//         if (buffer1.readyForWriting) {
//             buffer1isEmpty = true;
//         } else {
//             buffer1isEmpty = false;
//         }
//     }
//     {
//         std::unique_lock lock(buffer2.mtx);
//         if (buffer2.readyForWriting) {
//             buffer2isEmpty = true;
//         } else {
//             buffer2isEmpty = false;
//         }
//     }

//     if (isReadingFinished && buffer1isEmpty && buffer2isEmpty) {
//         return true;
//     } else {
//         return false;
//     }
// }
