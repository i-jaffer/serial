#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include "serial.h"

int main()
{
        int ret = 0;
        int fd;
        char buf[BUFSIZ] = {0};
        fd = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY);
        if(fd == -1) {
                perror("open ttyusb0 error!");
                exit(-1);
        }

        serial_attr_struct serial_attr;
        serial_attr.databits = 8;
        serial_attr.stopbits = 1;
        serial_attr.parity = PARITY_NO;
        serial_attr.echo_switch = ECHO_CLOSE;

        serial_mode_struct serial_mode;
        serial_mode.vtime_100ms = 0;
        serial_mode.vmin = 1;

        serial_set_speed(fd, 115200);
        serial_set_attribute(fd, serial_attr);
        serial_set_mode(fd, serial_mode);

#if 1
        while(1) {
                ret = read(fd, buf, sizeof(buf));
                if(ret == -1) {
                        perror("read error!");
                        exit(-1);
                }
                else {
                        printf("read num:%d\n", ret);
                        printf("buf:%s\n", buf);
                }
                memset(buf, 0, sizeof(buf));
        }
#else
        fd_set rset;
        char r_buf[BUFSIZ];
        while(1)
        {
                FD_ZERO(&rset) ;
                FD_SET(fd, &rset) ;
                ret = select(fd+1, &rset, NULL, NULL, NULL) ;
                if(ret < 0)
                {
                        printf("select() failed: %s\n", strerror(errno)) ;
                        goto out ;
                }
                if(ret == 0)
                {
                        printf("select() time out!\n") ;
                        goto out ;
                }

                memset(r_buf, 0, sizeof(r_buf)) ;
                ret = read(fd, r_buf, sizeof(r_buf)) ;
                if(ret < 0)
                {
                        printf("Read() error:%s\n",strerror(errno)) ;
                        goto out ;
                }
                printf("Read from tty: %s\n",r_buf) ;
        }
#endif
out:
        close(fd);
        return 0;
}

