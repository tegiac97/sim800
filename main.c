#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <string.h>

void main(void)
{
    int fd; /*File Descriptor*/

    printf("\n +----------------------------------+");
    printf("\n |        Serial Port Write         |");
    printf("\n +----------------------------------+");

    /*------------------------------- Opening the Serial Port -------------------------------*/

    /* Change /dev/ttyUSB0 to the one corresponding to your system */

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY ); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
                                                             /* O_RDWR Read/Write access to serial port           */
                                                             /* O_NOCTTY - No terminal will control the process   */
                                                             /* O_NDELAY -Non Blocking Mode,Does not care about-  */
                                                             /* -the status of DCD line,Open() returns immediatly */

    if (fd == -1) /* Error Checking */
        printf("\n  Error! in Opening ttyUSB0  ");
    else
        printf("\n  ttyUSB0 Opened Successfully ");

    /*---------- Setting the Attributes of the serial port using termios structure --------- */

    struct termios sim800l; /* Create the structure                          */

    tcgetattr(fd, &sim800l); /* Get the current attributes of the Serial port */

    cfsetispeed(&sim800l, B115200); /* Set Read  Speed as 9600                       */
    cfsetospeed(&sim800l, B115200); /* Set Write Speed as 9600                       */

    sim800l.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
    sim800l.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    sim800l.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
    sim800l.c_cflag |= CS8;     /* Set the data bits = 8                                 */

    sim800l.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    sim800l.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

    sim800l.c_iflag &= ~(IXON | IXOFF | IXANY);         /* Disable XON/XOFF flow control both i/p and o/p */
    sim800l.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

    sim800l.c_oflag &= ~OPOST; /*No Output Processing*/

    sim800l.c_cc[VMIN] = 128; /* Read at least 10 characters */
    sim800l.c_cc[VTIME] = 2; /* Wait indefinetly   */

    if ((tcsetattr(fd, TCSANOW, &sim800l)) != 0) /* Set the attributes to the termios structure*/
        printf("\n  ERROR ! in Setting attributes");
    else
        printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

    /*------------------------------- Write data to serial port -----------------------------*/

    char write_buffer[] = "AT+CCID"; /* Buffer containing characters to write into port	     */
    int bytes_written = 0;             /* Value for storing the number of bytes written to the port */

    bytes_written = write(fd, write_buffer, sizeof(write_buffer)); /* use write() to send data to port                                            */
                                                                   /* "fd"                   - file descriptor pointing to the opened serial port */
                                                                   /*	"write_buffer"         - address of the buffer containing data	            */
                                                                   /* "sizeof(write_buffer)" - No of bytes to write                               */
    printf("\n  %s written to ttyUSB0", write_buffer);
    printf("\n  %d Bytes written to ttyUSB0", bytes_written);
    printf("\n +----------------------------------+\n\n");

    /*------------------------------- Read data from serial port -----------------------------*/
    tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */

    char read_buffer[32]; /* Buffer to store the data received              */
    int bytes_read = 0;   /* Number of bytes read by the read() system call */
    int i = 0;

    bytes_read = read(fd, &read_buffer, 32); /* Read the data                   */
    printf("%s\n", strerror(errno));

    printf("\n\n  Bytes Rxed -%d", bytes_read); /* Print the number of bytes read */
    printf("\n\n  ");
    printf("%s\n", read_buffer);

    // for (i = 0; i < bytes_read; i++) /*printing only the received characters*/
    //     printf("%c", read_buffer[i]);

    printf("\n +----------------------------------+\n\n\n");

    close(fd); /* Close the Serial port */
}