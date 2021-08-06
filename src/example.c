#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include "serial.h"

int serial_init(int fd)
{
        int ret = 0;

        serial_attr_struct serial_attr;
        serial_attr.databits = 8;
        serial_attr.stopbits = 1;
        serial_attr.parity = PARITY_NO;
        serial_attr.echo_switch = ECHO_CLOSE;

        serial_mode_struct serial_mode;
        serial_mode.vtime_100ms = 10;
        serial_mode.vmin = 1;

        ret = serial_set_speed(fd, 576000);
        if (ret == -1) {
                fprintf(stderr, "serial set speed error!\n");
                goto out;
        }
        ret = serial_set_attribute(fd, serial_attr);
        if (ret == -1) {
                fprintf(stderr, "serial set attribute error!\n");
                goto out;
        }
        ret = serial_set_mode(fd, serial_mode);
        if (ret == -1) {
                fprintf(stderr, "serial set mode error!\n");
                goto out;
        }

out:
        return ret;
}

int output_check(int fd)                                                                
{
        int ret = 0;
        char buf[256] = {0};

        for(int i=0; i<256; i++)
                buf[i] = i;

        ret = write(fd, buf, 256);
        if(ret == -1) {
                perror("write error");
                goto err;
        }
err:
        return ret;
}

int main()
{
        int ret = 0;
        int fd;
        unsigned char buf[BUFSIZ] = {0};
        fd = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY);
        if (fd == -1) {
                perror("open ttyusb0 error");
                exit(-1);
        }
        serial_init(fd);
        output_check(fd);

        while (1) {
                ret = read(fd, buf, sizeof(buf));
                if (ret == -1) {
                        perror("read error");
                        exit(-1);
                }
                else {
                        for(int i=0; i<ret; i++)
                                printf(" %02x", buf[i]);
                        fflush(stdout);
                }
                memset(buf, 0, sizeof(buf));
        }

out:
        ret = close(fd);
        if (ret == -1) {
                perror("close error");
                exit(-1);
        }

        return 0;
}

