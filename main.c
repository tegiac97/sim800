#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> // needed for memset
#include <errno.h>

int main(int argc, char **argv)
{
        struct termios tio;
        struct termios stdio;
        int tty_fd;
        fd_set rdset;

        char c='\0';

        printf("Please start with %s /dev/ttyS1 (for example)\n", argv[0]);
        memset(&stdio, 0, sizeof(stdio));
        stdio.c_iflag = 0;
        stdio.c_oflag = 0;
        stdio.c_cflag = 0;
        stdio.c_lflag = 0;
        stdio.c_cc[VMIN] = 1;
        stdio.c_cc[VTIME] = 0;
        tcsetattr(STDOUT_FILENO, TCSANOW, &stdio);
        tcsetattr(STDOUT_FILENO, TCSAFLUSH, &stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // make the reads non-blocking

        memset(&tio, 0, sizeof(tio));
        tio.c_iflag = 0;
        tio.c_oflag = 0;
        tio.c_cflag = CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
        tio.c_lflag = 0;
        tio.c_cc[VMIN] = 2;
        tio.c_cc[VTIME] = 20;

        tty_fd = open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK); // O_NONBLOCK might override VMIN and VTIME, so read() may return immediately.
        cfsetospeed(&tio, B115200);                           // 115200 baud
        cfsetispeed(&tio, B115200);                           // 115200 baud

        tcsetattr(tty_fd, TCSANOW, &tio);

        while (c!='`')
        {
                // memset(c, 0, sizeof(c));
                // fflush(stdin);
                // fgets(c,10,stdin);
                // printf("%s\r\n", strerror(errno));
                if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
                if (read(STDIN_FILENO,&c,1)>0)  write(tty_fd,&c,1);                     // if new data is available on the console, send it to the serial port
        }

        close(tty_fd);
}
