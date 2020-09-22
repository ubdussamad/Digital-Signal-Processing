#include "CircularBuffer.h"
#include <iostream>
/**
 * @breif: Initialize a ring buffer of certain width.
 * @params: [in] bufferWidth, width or length of the buffer.
 */ 
template <typename T>
circularBuffer<T>::circularBuffer (unsigned int bufferWidth_ ) :
    bufferWidth(bufferWidth_) {
        this->bufferPointer = new T[bufferWidth_]; // Assign int a block of contigious memory.
        this->bufferFull    = false; // Since the buffer is empty initially.
        // Initially both the read and write pointers are
        // pointing to the same loc as the ring(buffer) is empty.
        //std::cout << "starting address of the poointer is: " << this->bufferPointer << " and ending addr is: " << this->bufferPointer+(bufferWidth - 1) << " \n";
        this->readPointer   = this->bufferPointer;
        this->writePointer  = this->bufferPointer;
    }

/**
 * @breif: Get the width this cicular buffer..
 * @return: [out] Buffer width or length of the buffer.
 */ 
template <typename T>
unsigned int circularBuffer<T>::getBufferWidth () {
    return this->bufferWidth;
}

template <typename T>
/**
 * @breif: Write to the cicular buffer..
 * @param: [in] Element to write. 
 */ 
void circularBuffer<T>::write(const T value) {

    //std::cout << "The write pointer is at: " << this->writePointer << "\n";
    
    *this->writePointer = value;

    if ((this->readPointer == this->writePointer) && this->bufferFull) {
        //std::cout << "Incrementing read pointer since the ring is full!\n";
        //std::cout << "The READ pointer was at: " << this->readPointer << "\n";
        if ( this->readPointer == (this->bufferPointer + (this->bufferWidth-1)) ) {
            this->readPointer = this->bufferPointer;
        }
        else {
        readPointer++;
        }
        //std::cout << "The READ pointer is at: " << this->readPointer << "\n";
    }

    
    if ( this->writePointer == (this->bufferPointer + (this->bufferWidth-1)) ) {
        /** Case when the ring is fully written and we start from the
         * begining.
         * Note: there might be need for a feature that if the ring is full
         * the write operation locks until the read operation has read that
         * block.
         * This might be useful in some cases, but as if now there is no intent
         * of implimenting it.
         */
        this->writePointer = this->bufferPointer; // Shift the write pointer to the initial.
        this->bufferFull = true;
        //std::cout << "The write pointer is back to: " << this->writePointer << "\n";
    }
    else {
        this->writePointer++;
    }



    return;
}

/**
 * @breif: Read from the cicular buffer..
 * @return: [out] Last unread element. 
 */ 
template <typename T>
T circularBuffer<T>::read() {
    T returnValue = *(this->readPointer);


    if (this->readPointer == (this->bufferPointer + (this->bufferWidth-1)) ) {
        /** Case when the ring is fully read and we start from the
         * begining.
         * Note: there might be need for a feature that if the ring is full
         * the write operation locks until the read operation has read that
         * block.
         * This might be useful in some cases, but as if now there is no intent
         * of implimenting it.
         */
        this->readPointer = this->bufferPointer;
        this->bufferFull = false;
    }
    else {
        this->readPointer++;
    }
    return returnValue;
}

/**
 * @breif: Checks if the buffer is empty?
 * @return: [out] if it's empty return true else false.
 */ 
template <typename T>
bool circularBuffer<T>::isEmpty() {
    //  Check if write pointer is ahead of read?
    if (this->writePointer > (this->readPointer)) { // this comparison can be made cause the memory alloc is contigious.
        return(false);
    }
    // Check if buffer is full and read leads write
    else if ( (this->readPointer >= this->writePointer) && this->bufferFull ) {
        return (false);
    }
    else {
        return (true);
    }
}

/**
 * @breif: Print the whole buffer to stdout.
 * 
 * this methods dosen't cares if the buffer is full
 * or empty, neither does it calls the read() call,
 * it just harmlessly prints the whole buffer for debugging
 * porposes.
 */
template <typename T>
void circularBuffer<T>::printBuffer(void) {
    std::cout << "\n[ ";
    for (unsigned int iter(0); iter < this->bufferWidth; iter++ ) {
        std::cout << *(this->bufferPointer+iter) << ", ";
    }
    std::cout << "\b \b]\n";
}



/**This is instantiated so that I can
 * put the definition of tempalted class
 * functions  into a seprate cpp file and
 * avoid linker errors in doing so.
 */
// template class circularBuffer<unsigned int>;