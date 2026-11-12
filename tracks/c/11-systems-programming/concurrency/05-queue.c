/* A bounded producer-consumer queue protected by a mutex and conditions. */

#include "bounded_queue.h"

#include <stdio.h>
#include <stdlib.h>

#define ITEM_COUNT 20

static void *produce(void *context) {
    struct queue *queue = context;
    for (int value = 0; value < ITEM_COUNT; ++value) {
        if (!enqueue(queue, value)) {
            break;
        }
    }
    queue_close(queue);
    return NULL;
}

static void *consume(void *context) {
    struct queue *queue = context;
    int value;
    while (dequeue(queue, &value)) {
        printf("%d ", value);
    }
    return NULL;
}

int main(void) {
    struct queue queue;
    pthread_t producer;
    pthread_t consumer;

    if (queue_init(&queue) != 0) {
        fputs("Unable to initialize queue synchronization\n", stderr);
        return 1;
    }
    if (pthread_create(&producer, NULL, produce, &queue) != 0) {
        fputs("Unable to create producer thread\n", stderr);
        queue_destroy(&queue);
        return 1;
    }
    if (pthread_create(&consumer, NULL, consume, &queue) != 0) {
        fputs("Unable to create consumer thread\n", stderr);
        queue_close(&queue);
        pthread_join(producer, NULL);
        queue_destroy(&queue);
        return 1;
    }

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    putchar('\n');
    queue_destroy(&queue);
    return 0;
}
