#ifndef __QUEUE_H_
#define __QUEUE_H_

#include <pthread.h>

typedef struct queue_stuct {
        int *queue;
        int size;
        int index;
        int vaild_num;
        pthread_mutex_t mutex;
}queue_struct_typedef;


int queue_init(queue_struct_typedef *queue_struct, int size);
int queue_deinit(queue_struct_typedef *queue_struct);
int queue_in(queue_struct_typedef *queue_struct, int data);
int queue_out(queue_struct_typedef *queue_struct);

#endif
