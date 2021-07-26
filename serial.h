#ifndef __SERIAL_H_

typedef enum parity_enum_typedef {
        PARITY_NO = 0,
        PARITY_ODD,
        PARITY_EVEN
} parity_enum;

typedef enum echo_switch_typedef {
        ECHO_CLOSE = 0,
        ECHO_OPEN
} echo_switch_enum;

typedef struct serial_attr_typedef{
        int databits;
        int stopbits;
        parity_enum parity;
        echo_switch_enum echo_switch;
} serial_attr_struct;

typedef struct serial_mode_typedef {
        unsigned char vtime_100ms;
        unsigned char vmin;
} serial_mode_struct;

int serial_set_speed(int fd, int speed);
int serial_set_attribute(int fd, serial_attr_struct serial_attr);
int serial_set_mode(int fd, serial_mode_struct serial_mode);

#endif
