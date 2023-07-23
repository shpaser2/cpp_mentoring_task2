#include <thread>
#include "rw.h"
#include <iostream>

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
