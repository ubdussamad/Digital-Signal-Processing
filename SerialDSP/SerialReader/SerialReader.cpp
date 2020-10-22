/**
 * SerialReader.cpp
 * A GNU Linux/Unix Project.
 * Ref: Sep 22, 2020
 * Author: ubdussamad <ubdussamad@gmail.com>
 * Based on the tutorial: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 */



#include "SerialReader.hpp"
#include <iostream>




serialReader::serialReader ( const char *  file_, int accessFlag = O_RDONLY ) : file(file_)
{
    this->errcode = 0b00000000;
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
        cfsetispeed(&this->tty,  B115200);//baudrate);
        cfsetospeed(&this->tty,  B115200);//baudrate);
        X_ERROR("Setting baudrate to: %d" , baudrate);
        // Save this->tty settings, also checking for error
        if (tcsetattr(this->serialDesc, TCSANOW, &this->tty) != 0) {
            this->errcode |= 0b00000100;
            X_ERROR("%i from tcsetattr: %s\n", errno, strerror(errno));
            return (this->errcode);
        }
        return (this->errcode);

    }

int serialReader::readNumericBuffer(double* buffer, unsigned int valueCount,
                                    unsigned int maxReads, size_t strBufferSize) {
    char read_buf [strBufferSize+1];
    int reads = valueCount;
//    std::cout << "Reading from Serial....\n" << std::endl;
    while (maxReads) {
        memset(&read_buf, '\0', strBufferSize);
//        std::cout << "Read blocked!\n" << std::endl;
        size_t num_bytes = read( this->serialDesc , &read_buf, strBufferSize);
//        std::cout << "Read Released." << std::endl;
        // if (!num_bytes) {continue;}
        // std::cout << maxReads << "|Read buffer is: " << read_buf << "\n|";
        double bvalue = std::atof(read_buf);
        double zero = 0;
        if (bvalue != zero) {valueCount--;buffer[valueCount] = bvalue;}
            //    std::cout << "\n" <<  maxReads << " Value: " << bvalue << std::endl;

        maxReads--;
    }
    close(this->serialDesc);
//    std::cout << "Done reading Serial!\n" << std::endl;
    return reads-valueCount;
}

void serialReader::fillNumBuffer (double * numBuffer , char * charBuffer , int count ) {
    size_t num_bytes = 0;

    while (count) {
        memset(charBuffer , '\0' ,1024);
        num_bytes = read( this->serialDesc , charBuffer, count);
    }
    
}

size_t serialReader::readSerial(char * buffer , size_t size) {
    memset(buffer ,'\0',size);
    return read( this->serialDesc , buffer, size);
}

// int main (  void  ) {
//     serialReader sr("/dev/ttyACM0");
//     if (sr.setSerialProperties(115200,true,1,6)) {
//         X_ERROR("Error configuring file.\n");
//         return(0);
//     }
//     char buff[8];
//     X_INFO("Hi");
//     std::cout << std::endl;
//     memset(buff, '\0', 8);
//     while (1)
//     {
//         sr.fillCharBuffer(buff , 8);
//         std::cout << "-------\n" << buff << "-------\n" << std::endl;
//     }
    


    // while (1) {
    //     sleep(1);
    //     sr.readNumericBuffer(buff, 2 , 2 , 20);

    //     for ( int i =0; i <2; i++) {
    //         X_PRINTF("%d: %f" , i,buff[i]);
    //         std::cout << std::endl;
    //     }

    //     std::cout << "Buffer ends here...." << std::endl;
    // }
//}
