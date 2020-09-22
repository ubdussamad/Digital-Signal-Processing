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
#include "./Circular Buffer/CircularBuffer.h"

int setTtyTermios (bool lineEnding , int baudrate);
int serial_port = open("/dev/ttyACM0", O_RDONLY);

class serialReader {

    public:
        serialReader( const char * file , int accessFlag );
        int  setSerialProperties ( bool readTilllineEnds, int baudrate);
        //void fillRingBuffer( circularBuffer * cbuf ,);
        
        template <class T>
        T readNumericBuffer(int valueCount);

        template <class T>
        T readNumeric (void);

        const char * read_raw();


    private:
        const char * file;
        struct termios tty; // Declaring the tty struct.




};

int serialReader::setSerialProperties ( bool readTilllineEnds , int baudrate ) {
        
        // Read in existing settings, and handle any error
        if ( tcgetattr(serial_port, &tty) != 0 ) {
            printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
            return -1;
        }

        tty.c_cflag &= ~PARENB;         // Clear parity bit, disabling parity (most common)
        tty.c_cflag &= ~CSTOPB;         // Clear stop field, only one stop bit used in communication (most common)
        tty.c_cflag |= CS8;             // 8 bits per byte (most common)
        tty.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control (most common)
        tty.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1)


        // Input is processed when a new line character is received.
        if (readTilllineEnds) { 
            tty.c_lflag |= ICANON;
        }
        else {
            tty.c_lflag &= ~ICANON;
        }

        tty.c_lflag &= ~ECHO; // Disable echo
        tty.c_lflag &= ~ECHOE; // Disable erasure
        tty.c_lflag &= ~ECHONL; // Disable new-line echo
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes


        // TODO: Checkn this out jimmy.
        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        // TODO: Change this setting to manipulate data.
        tty.c_cc[VTIME] = 100;    // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
        tty.c_cc[VMIN] = 50; // Wait (block) till 5 character are recived and then release 

        // // Set in/out baud rate to be 9600
        // cfsetispeed(&tty, B9600);
        // cfsetospeed(&tty, B9600);

        cfsetispeed(&tty, baudrate);
        cfsetospeed(&tty, baudrate);

        // Save tty settings, also checking for error
        if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
            return(-1);
        }
        return (0);

    }


template <typename T>
void read(int bufferSize) {
    // Allocate memory for read buffer, set size according to your needs
    char read_buf [50];
    // Normally you wouldn't do this memset() call, but since we will just receive
    // ASCII data for this example, we'll set everything to 0 so we can
    // call printf() easily.
    unsigned long bufferSize  = sizeof(read_buf);
    while (1) {
        serial_port = open("/dev/ttyACM0", O_RDWR);
        memset(&read_buf, '\0', bufferSize);
        // Read bytes. The behaviour of read() (e.g. does it block?,
        // how long does it block for?) depends on the configuration
        // settings above, specifically VMIN and VTIME
        int num_bytes = read(serial_port, &read_buf, bufferSize);


        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes < 0) {
            continue;
        }

        if ( std::atof( read_buf) != -1) {
            printf("The value recieved is: %f \n" , std::atof( read_buf) );
        }
        close(serial_port);
    }
    }
