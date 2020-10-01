/**
 * SerialReader.cpp
 * A GNU Linux/Unix Project.
 * Ref: Sep 22, 2020
 * Author: ubdussamad <ubdussamad@gmail.com>
 * Based on the tutorial: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 */


// C library headers
#include <stdio.h>
#include <string.h>
#include <cstdlib>


// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include "Circular Buffer/CircularBuffer.h"

int setTtyTermios (bool lineEnding , int baudrate);
int serial_port = open("/dev/ttyACM0", O_RDONLY);

class serialReader {

    public:
        serialReader( const char * file_ , int accessFlag );
        int  setSerialProperties ( bool readTilllineEnds, int baudrate);
        //void fillRingBuffer( circularBuffer * cbuf ,);
        
        template <class T>
        void readNumericBuffer(T& buffer, int valueCount, int strBufferSize = 50, int maxReads = 10);

    private:
        const char * file;
        struct termios tty; // Declaring the tty struct
};

serialReader::serialReader(const char *  file , int accessFlag ) {
    
}

int serialReader::setSerialProperties ( bool readTilllineEnds , int baudrate ) {
        
        // Read in existing settings, and handle any error
        if ( tcgetattr(serial_port, &this->tty) != 0 ) {
            printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
            return -1;
        }
        this->tty.c_cflag &= ~PARENB;         // Clear parity bit, disabling parity (most common)
        this->tty.c_cflag &= ~CSTOPB;         // Clear stop field, only one stop bit used in communication (most common)
        this->tty.c_cflag |= CS8;             // 8 bits per byte (most common)
        this->tty.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control (most common)
        this->tty.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1)
        // Input is processed when a new line character is received.
        if (readTilllineEnds) { this->tty.c_lflag |= ICANON;}
        else {this->tty.c_lflag &= ~ICANON;}
        this->tty.c_lflag &= ~ECHO; // Disable echo
        this->tty.c_lflag &= ~ECHOE; // Disable erasure
        this->tty.c_lflag &= ~ECHONL; // Disable new-line echo
        this->tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        this->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        this->tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
        // TODO: Checkn this out jimmy.
        this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // this->tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // this->tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        // TODO: Change this setting to manipulate data.
        this->tty.c_cc[VTIME] = 100;    // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
        this->tty.c_cc[VMIN] = 50; // Wait (block) till 5 character are recived and then release 
        cfsetispeed(&this->tty, baudrate);
        cfsetospeed(&this->tty, baudrate);
        // Save this->tty settings, also checking for error
        if (tcsetattr(serial_port, TCSANOW, &this->tty) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
            return(-1);
        }
        return (0);

    }


template <typename T>
void serialReader::readNumericBuffer<T>(T& buffer, int valueCount, int strBufferSize, int maxReads) {
    char read_buf [strBufferSize];
    serial_port = open("/dev/ttyACM0", O_RDWR);

    while (maxReads--) {
        memset(&read_buf, '\0', strBufferSize);
        int num_bytes = read( this->seralDesc , &read_buf, strBufferSize);
        if (num_bytes < 0) {continue;}
        
        switch (typeid(T)) {
            case typeid(double):
                buffer[maxReads-1] = std::atof(read_buf);
                break;

            case typeid(int):
                buffer[maxReads-1] = std::atoi(read_buf);
                break;
            
            default:
                break;
        }
    }
    close(serial_port);
}
