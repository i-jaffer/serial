#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include "serial.h"
#include "queue.h"

sem_t sem_receive;
queue_struct_typedef receive_len_fifo;

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

        write(fd, "hello", 6);
out:
        return ret;
}

void *receive_statistic(void *arg)
{
        int total_len = 0;
        int ret = 0;
        while (1) {
#if 0
                ret = sem_wait(&sem_receive);
                if (ret == -1) {
                        perror("sem_wait error");
                        exit(-1);
                }
                total_len += queue_out(&receive_len_fifo);
                //printf("\n******** total_len=%d ***********\n", total_len);
#endif
        }
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
        queue_init(&receive_len_fifo, 10);

        ret = sem_init(&sem_receive, 0, 0);
        if (ret == -1) {
                perror("sem init error");
                exit(-1);
        }
        
        pthread_t tid;
        ret = pthread_create(&tid, NULL, receive_statistic, NULL);
        if (ret == -1) {
                perror("pthread create error");
                exit(-1);
        }
        ret = pthread_detach(tid);
        if (ret != 0) {
                fprintf(stderr, "pthread detach error!\n");
                exit(-1);
        }

        while (1) {
                int len = 0;
                ret = read(fd, buf, sizeof(buf));
                if (ret == -1) {
                        perror("read error");
                        exit(-1);
                }
                else {
                        for(int i=0; i<ret; i++) {
                                printf(" %02x", buf[i]);
                                len ++;
                        }
                        fflush(stdout);
                        //printf("\n");
#if 0
                        while(queue_in(&receive_len_fifo, len) == -1) {
                                printf("queue_in error!\n");
                                sleep(1);
                        }
                        ret = sem_post(&sem_receive);
                        if (ret == -1) {
                                perror("sem post error!");
                                exit(-1);
                        }
#endif
                }
                memset(buf, 0, sizeof(buf));
        }

out:
        queue_init(&receive_len_fifo, 0);

        ret = sem_destroy(&sem_receive);
        if (ret == -1) {
                perror("sem receive destroy error");
                exit(-1);
        }

        ret = close(fd);
        if (ret == -1) {
                perror("close error");
                exit(-1);
        }

        return 0;
}

