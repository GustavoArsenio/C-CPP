#ifndef BOUNDED_QUEUE_H
#define BOUNDED_QUEUE_H

#include <pthread.h>
#include <stddef.h>

#define QUEUE_CAPACITY 10

struct queue {
    int values[QUEUE_CAPACITY];
    size_t head;
    size_t tail;
    size_t count;
    int closed;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

/* Initialize once; destroy only after all users have stopped. */
int queue_init(struct queue *queue);
void queue_destroy(struct queue *queue);
/* Operations block while full/empty; return 0 after close or for bad pointers. */
int enqueue(struct queue *queue, int value);
int dequeue(struct queue *queue, int *value);
void queue_close(struct queue *queue);

#endif
