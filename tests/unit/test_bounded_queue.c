#include <assert.h>

#include "bounded_queue.h"

int main(void) {
    struct queue queue;
    int value = -1;
    assert(queue_init(NULL) == -1);
    assert(queue_init(&queue) == 0);
    assert(!dequeue(&queue, NULL));
    assert(enqueue(&queue, 11));
    assert(dequeue(&queue, &value) && value == 11);
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        assert(enqueue(&queue, i));
    }
    /* Closing releases any full-queue wait and rejects later writes. */
    queue_close(&queue);
    assert(!enqueue(&queue, 99));
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        assert(dequeue(&queue, &value) && value == i);
    }
    /* A closed queue drains existing data, then reports empty. */
    assert(!dequeue(&queue, &value));
    assert(!enqueue(NULL, 1));
    assert(!dequeue(NULL, &value));
    queue_close(NULL);
    queue_destroy(&queue);
    queue_destroy(NULL);
    return 0;
}
