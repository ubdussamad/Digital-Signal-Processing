// C library headers
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <typeinfo>
#include <iostream>


// lOGGING
#define SHELL_BASH      1
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
        void fillNumBuffer (double * numBuffer , char * charBuffer , int count  = 1024);
        size_t readSerial(char * buffer , size_t size);
        int readNumericBuffer(double* buffer,unsigned int valueCount,unsigned int maxReads = 5,size_t strBufferSize = 4);

    protected:
        int serialDesc;    // Serial port file descriptor.
        const char * file;
        struct termios tty; // Declaring the tty struct
        short errcode;


        // MSB -> NU, NU, NU, NU, NU, Error writing temios struct for fd, Error reading termios struct for fd , Error Opening File descritor.
};


