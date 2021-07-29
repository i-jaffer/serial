#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <string.h>
#include "serial.h"

/*
 * @brief 设置串口通讯波特率
 * @param fd：串口设备文件描述符
 * @param speed：串口通讯波特率
 * @retval 0:success -1:error
 */
int serial_set_speed(int fd, int speed)
{
        int speed_arr[] = { B50, B75, B110, B134, B150, B200,
                B300, B600, B1200, B1800, B2400, B4800, B9600,
                B19200, B38400, B57600, B115200, B230400,
                B460800, B500000, B576000, B921600, B1000000,
                B1152000, B1500000, B2000000, B2500000, B3000000,
                B3500000, B4000000};
        int name_arr[] = {50, 75, 110, 134, 150, 200, 300, 600,
                1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600,
                115200, 230400, 460800, 500000, 576000, 921600,
                1000000, 1152000, 1500000, 2000000, 2500000, 3000000,
                3500000, 4000000};
        int ret = 0;
        struct termios opt;
        memset(&opt, 0, sizeof(opt));

        ret = tcgetattr(fd, &opt);
        if(ret == -1) {
                perror("tcgetattr error in serial set speed");
                ret = -1;
                goto out;
        }
        for(int i=0; i<(sizeof(speed_arr) / sizeof(int)); i++) {
                if(name_arr[i] == speed) {
                        ret = tcflush(fd, TCIOFLUSH); /* 清空缓存区 */
                        if(ret == -1) {
                                perror("tcflush error in serial set speed");
                                goto out;
                        }
                        ret = cfsetispeed(&opt, speed_arr[i]);
                        if(ret == -1) {
                                perror("cfsetispeed error in serial set speed");
                                goto out;
                        }
                        ret = cfsetospeed(&opt, speed_arr[i]);
                        if(ret == -1) {
                                perror("cfsetospeed error in serial set speed");
                                goto out;
                        }
                        ret = tcsetattr(fd, TCSANOW, &opt);
                        if(ret == -1) {
                                perror("tcsetattr error in serial set speed");
                                goto out;
                        }
                        ret = tcflush(fd, TCIOFLUSH);
                        if(ret == -1) {
                                perror("tcflush error in serial set speed");
                                goto out;
                        }
                }
        }

out:
        return ret;
}

/*
 * @brief 设置串口属性
 * @param fd:文件描述符
 * @param serial_attr:串口配置属性结构体 @ref serial_attr_struct
 * @retval 0:success 1:failed
 */
int serial_set_attribute(int fd, serial_attr_struct serial_attr)
{
        int ret = 0;
        struct termios opt;
        memset(&opt, 0, sizeof(opt));

        ret = tcgetattr(fd, &opt);
        if(ret == -1) {
                perror("tcgetattr error in serial set attribute");
                goto out;
        }

        opt.c_cflag &= ~CSIZE;
        switch (serial_attr.databits) {
        case 7:
                opt.c_cflag |= CS7;
                break;
        case 8:
                opt.c_cflag |= CS8;
                break;
        default:
                fprintf(stderr, "unsupport data size\n");
                ret = -1;
                goto out;
        }

        switch (serial_attr.parity) {
        case PARITY_NO:
                opt.c_cflag &= ~PARENB; /* disable parity generation on output and checking for input */
                opt.c_iflag &= ~INPCK;  /* disable input parity checking */
                break;
        case PARITY_ODD:
                opt.c_cflag |= (PARODD | PARENB);/* set parity is odd, and enable parity */
                opt.c_iflag |= INPCK;   /* enable input parity checking */
                break;
        case PARITY_EVEN:
                opt.c_cflag |= PARENB;  /* enable parity generation on output and parity checking for input */
                opt.c_cflag &= ~PARODD; /* set parity is even */
                opt.c_iflag |= INPCK;   /*enable input parity checking*/ 
                break;
        default:
                fprintf(stderr, "unsupport parity\n");
                ret = -1;
                goto out;
        }

        switch (serial_attr.stopbits) {
        case 1:
                opt.c_cflag &= ~CSTOPB;
                break;
        case 2:
                opt.c_cflag |= CSTOPB;
                break;
        default:
                fprintf(stderr, "unsupport stopbits\n");
                ret = -1;
                goto out;
        }

        /* 回显设置 */
        switch (serial_attr.echo_switch) {
        case ECHO_CLOSE:
                opt.c_lflag &= ~ECHO;
                opt.c_lflag &= ~ECHONL;
                break;
        case ECHO_OPEN:
                opt.c_lflag |= ECHO;
                opt.c_lflag |= ECHONL;
                break;
        }

        /* 禁用输出处理 */
        opt.c_oflag &= ~OPOST;

        ret = tcsetattr(fd, TCSANOW, &opt);
        if(ret == -1)
                perror("tcssetattr error in serial set attribute");

        ret = tcflush(fd, TCIOFLUSH); /* 清空缓存区 */
        if(ret == -1)
                perror("tcflush error in serial set attribute");
        
out:
        return ret;
}

/*
 * @brief 设置串口read模式
 * @param fd:对应串口文件描述符
 * @param serial_mode_struct:设置串口read模式 @ref serial_mode_struct
 * @retval 0:success -1:failed
 */
int serial_set_mode(int fd, serial_mode_struct serial_mode)
{
        int ret = 0;
        struct termios opt;
        memset(&opt, 0, sizeof(opt));

        ret = tcgetattr(fd, &opt);
        if(ret == -1) {
                perror("tcgetattr error in serial set mode");
                goto out;
        }

        /* 设置read是否阻塞等 详见man termios /c_cc */
        /* 设置计时时间time_100ms*(1/10)s */
        opt.c_cc[VTIME] = serial_mode.vtime_100ms;
        /* 设置读取的最小字符数 */
        opt.c_cc[VMIN] = serial_mode.vmin;

        ret = tcflush(fd, TCIOFLUSH); /* 清空缓存区 */
        if(ret == -1)
                perror("tcflush error in serial set mode");

        ret = tcsetattr(fd, TCSANOW, &opt);
        if(ret == -1)
                perror("tcssetattr error in serial set mode");
out:
        return ret;
}

