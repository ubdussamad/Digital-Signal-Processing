#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// class x { 
//     public:
//         x () {
//             std::cout << "Constructor Called!" << std::endl;
//         }
//         ~x() {
//             std::cout <<  "Destructor called!" << std::endl;
//         }
//         void print() {};
// };

// std::shared_ptr<x> fn () {
//     std::shared_ptr<x> rval = std::make_shared<x>();
//     return rval;
// }

void fillRing ( void ) {
    while (1) {
        sleep(5);
        std::cout << "Hey ya'll, sup?\n";
    }
}

int main ( void ) {
    std::thread ringBufferThread(fillRing);
    while (1) {
    sleep(2);
    std::cout << "Nothing much, hbu?\n";
    }
    
}