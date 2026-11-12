#include "bounded_queue.h"

int queue_init(struct queue *queue) {
    if (queue == NULL) {
        return -1;
    }
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    queue->closed = 0;
    if (pthread_mutex_init(&queue->mutex, NULL) != 0) {
        return -1;
    }
    if (pthread_cond_init(&queue->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&queue->mutex);
        return -1;
    }
    if (pthread_cond_init(&queue->not_full, NULL) != 0) {
        pthread_cond_destroy(&queue->not_empty);
        pthread_mutex_destroy(&queue->mutex);
        return -1;
    }
    return 0;
}

void queue_destroy(struct queue *queue) {
    if (queue == NULL) {
        return;
    }
    pthread_cond_destroy(&queue->not_full);
    pthread_cond_destroy(&queue->not_empty);
    pthread_mutex_destroy(&queue->mutex);
}

int enqueue(struct queue *queue, int value) {
    if (queue == NULL) {
        return 0;
    }
    pthread_mutex_lock(&queue->mutex);
    while (queue->count == QUEUE_CAPACITY && !queue->closed) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }
    if (queue->closed) {
        pthread_mutex_unlock(&queue->mutex);
        return 0;
    }
    queue->values[queue->head] = value;
    queue->head = (queue->head + 1) % QUEUE_CAPACITY;
    ++queue->count;
    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
    return 1;
}

int dequeue(struct queue *queue, int *value) {
    if (queue == NULL || value == NULL) {
        return 0;
    }
    pthread_mutex_lock(&queue->mutex);
    while (queue->count == 0 && !queue->closed) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    if (queue->count == 0) {
        pthread_mutex_unlock(&queue->mutex);
        return 0;
    }
    *value = queue->values[queue->tail];
    queue->tail = (queue->tail + 1) % QUEUE_CAPACITY;
    --queue->count;
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    return 1;
}

void queue_close(struct queue *queue) {
    if (queue == NULL) {
        return;
    }
    pthread_mutex_lock(&queue->mutex);
    queue->closed = 1;
    pthread_cond_broadcast(&queue->not_empty);
    pthread_cond_broadcast(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
}
