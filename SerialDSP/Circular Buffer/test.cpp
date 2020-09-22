#include <cstdlib>
#include <iostream>
#include <vector>
#include "CircularBuffer.h"
#include "CircularBuffer.cpp"



int main ( void ) {
    circularBuffer<unsigned int> ring(3);

    if (ring.isEmpty()) {
        std::cout << "Buffer is empty right now, pushing..\n\n";
    }


    // for ( int i=0; i < 10; i++) {
    //     ring.write(i);
    // }

    ring.write(1);
    ring.write(2);
    ring.write(3);
    ring.write(4);
    std::cout <<  ring.read() << "\n";
    std::cout <<  ring.read() << "\n";
    ring.write(5);
    // ring.write(6);
    // std::cout << "\n[ ";
    unsigned int i = ring.getBufferWidth();
    while /*(i--){//*/(!ring.isEmpty()) {
        // ring.printBuffer();
        std::cout << ring.read() << ",";
        // ring.read();
    }

    if (!ring.isEmpty() || 1) {
        ring.printBuffer();
    }

    return(0);

}
