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
#include <typeinfo>
#include <iostream>


// lOGGING
#define SHELL_BASH      0
#define DEBUG           1
// #define ENABLE_WARNING  0
// #define ENABLE_INFO     0
// #define ENABLE_ERROR    0
// #define ENABLE_SPECIAL  0
#include "logX.hpp"

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
// #include "CircularBuffer/CircularBuffer.h"



class serialReader {

    public:
        serialReader( const char * file_ , int accessFlag );
        int  setSerialProperties ( int baudrate, bool readintilllineEnds = false,unsigned int vtime = 60,unsigned int vmin = 4 );
        
        int readNumericBuffer(int* buffer,unsigned int valueCount,unsigned int maxReads = 5,size_t strBufferSize = 4);

    protected:
        int serialDesc;    // Serial port file descriptor.
        const char * file;
        struct termios tty; // Declaring the tty struct

        // MSB -> NU, NU, NU, NU, NU, Error writing temios struct for fd, Error reading termios struct for fd , Error Opening File descritor.
        short errcode = 0b00000000;
};

serialReader::serialReader ( const char *  file_, int accessFlag = O_RDONLY ) : file(file_)
{
    this->serialDesc = open(file_, accessFlag );
    X_INFO("Reading Port: ");
    X_PRINTF("%s\n" , file_);
    if (!serialDesc) {
        this->errcode |= 0b00000001;
    }
}

int serialReader::setSerialProperties ( int baudrate, bool readTilllineEnds, unsigned int vtime, unsigned int vmin ) {

        if ( this->errcode & 0b00000001 ) {
            X_ERROR("Opening the file descritor, file not found.");
        }
        
        // Read in existing settings, and handle any error
        if ( tcgetattr(this->serialDesc, &this->tty) != 0 ) {
            this->errcode |= 0b00000010;
            X_ERROR("Got %i from tcgetattr: %s\n", errno, strerror(errno));
            return (this->errcode);
        }
        this->tty.c_cflag &= ~PARENB;         // Clear parity bit, disabling parity (most common)
        this->tty.c_cflag &= ~CSTOPB;         // Clear stop field, only one stop bit used in communication (most common)
        this->tty.c_cflag |= CS8;             // 8 bits per byte (most common)
        this->tty.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control (most common)
        this->tty.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1)
        // Input is processed when a new line character is received.
        if (readTilllineEnds) { this->tty.c_lflag |= ICANON; X_WARN("Enabling RTLE."); }
        else {this->tty.c_lflag &= ~ICANON;}
        this->tty.c_lflag &= ~ECHO;     // Disable echo
        this->tty.c_lflag &= ~ECHOE;    // Disable erasure
        this->tty.c_lflag &= ~ECHONL;   // Disable new-line echo
        this->tty.c_lflag &= ~ISIG;     // Disable interpretation of INTR, QUIT and SUSP
        this->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        this->tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
        // TODO: Checkn this out jimmy.
        this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // this->tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // this->tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        // TODO: Change this setting to manipulate data.
        this->tty.c_cc[VTIME] = (unsigned char)vtime;    // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
        this->tty.c_cc[VMIN] =  (unsigned char)vmin; // Wait (block) till atleast 5 character are recived and then release 
        cfsetispeed(&this->tty,  baudrate);
        cfsetospeed(&this->tty,  baudrate);
        // Save this->tty settings, also checking for error
        if (tcsetattr(this->serialDesc, TCSANOW, &this->tty) != 0) {
            this->errcode |= 0b00000100;
            X_ERROR("%i from tcsetattr: %s\n", errno, strerror(errno));
            return (this->errcode);
        }
        return (this->errcode);

    }

int serialReader::readNumericBuffer(int* buffer, unsigned int valueCount, unsigned int maxReads, size_t strBufferSize) {
    char read_buf [strBufferSize];
    int reads = valueCount;
    X_INFO("Reading BUffer...\n");
    while (maxReads--) {
        memset(&read_buf, '\0', strBufferSize);
        size_t num_bytes = read( this->serialDesc , &read_buf, strBufferSize);
        if (num_bytes < 0U) {continue;}
        int i = std::atoi(read_buf);
        if (i != -1) {buffer[valueCount--] = i;}
    }
    close(this->serialDesc);
    return reads-valueCount;
}



int main (  void  ) {
    serialReader sr("/dev/ttyACM0");
    if (sr.setSerialProperties(115200,true,10,6)) {
        X_ERROR("Error configuring file.\n");
        return(0);
    }
    int buff[16];
    sr.readNumericBuffer(buff, 15 , 20 , 20);

    for ( int i =0; i <16; i++) {
        X_PRINTF("%d\n" , buff[i]);
    }

}