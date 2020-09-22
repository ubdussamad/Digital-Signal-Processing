// C library headers
#include <stdio.h>
#include <string.h>
#include <cstdlib>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
int serial_port = open("/dev/ttyACM0", O_RDONLY);
int find ( char * buff , char query );
// Create new termios struc, we call it 'tty' for convention
struct termios tty;


int main () {
    // sleep(5);
    // printf("Serial port opned.");
    // sleep(5);

    // Read in existing settings, and handle any error
    if ( tcgetattr(serial_port, &tty) != 0 ) {
        // printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    // printf("Terminos struct copied.");
    // sleep(5);

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    // printf("Maybe here?");
    // sleep(5);

    // tty.c_lflag &= ~ICANON;
    tty.c_lflag |= ICANON; // Experimently enabling this.

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
    tty.c_cc[VTIME] = 5;    // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 5; // Wait (block) till 5 character are recived and then release 

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // printf("Or here?");
    sleep(5);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        // printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    // Allocate memory for read buffer, set size according to your needs
    char read_buf [16];

    // Normally you wouldn't do this memset() call, but since we will just receive
    // ASCII data for this example, we'll set everything to 0 so we can
    // call printf() easily.

    
    unsigned long bufferSize  = sizeof(read_buf);
    while (1) {
        // printf("\n");
        serial_port = open("/dev/ttyACM0", O_RDWR);
        
        // printf("Or maybe after intial readin gthe file itself?");
        // sleep(5);
        
        memset(&read_buf, '\0', bufferSize);
        // Read bytes. The behaviour of read() (e.g. does it block?,
        // how long does it block for?) depends on the configuration
        // settings above, specifically VMIN and VTIME
        int num_bytes = read(serial_port, &read_buf, bufferSize);

        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes < 0) {
            // printf("Error reading: %s", strerror(errno));
            sleep(5);
            continue;
        }

        // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
        // print it to the screen like this!)
        // printf("Read %i bytes. Received message: %s\n\n", num_bytes, read_buf);
        // if (std::atof(read_buf) != -1) {
        //     printf("%f\n", std::atof(read_buf) );
        // }
        read_buf[find( read_buf , '\n')] = '\0';
        printf ( "\nxxxxx\n %s \nxxxxx\n" , read_buf );

        close(serial_port);
    }
}
int find ( char * buff , char query ) {
    for ( int i = 0; ;i++ ) {
        if (query==*buff) {
            return(i);
        }
        buff++;
    }
    return ( -1 );
}