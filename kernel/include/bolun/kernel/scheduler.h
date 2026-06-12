#ifndef BOLUN_KERNEL_SCHEDULER_H
#define BOLUN_KERNEL_SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOLUN_EARLY_RUN_QUEUE_CAPACITY 64U

typedef struct BolunThreadId {
    uint32_t value;
} BolunThreadId;

typedef struct BolunRunnableThread {
    BolunThreadId thread_id;
    uint8_t priority;
    uint64_t enqueued_at_nanoseconds;
} BolunRunnableThread;

typedef struct BolunEarlyRunQueue {
    BolunRunnableThread entries[BOLUN_EARLY_RUN_QUEUE_CAPACITY];
    bool occupied[BOLUN_EARLY_RUN_QUEUE_CAPACITY];
    size_t length;
} BolunEarlyRunQueue;

typedef enum BolunSchedulerStatus {
    BOLUN_SCHEDULER_STATUS_OK = 0,
    BOLUN_SCHEDULER_STATUS_BAD_ARGUMENT = 1,
    BOLUN_SCHEDULER_STATUS_QUEUE_FULL = 2,
    BOLUN_SCHEDULER_STATUS_EMPTY = 3
} BolunSchedulerStatus;

void BolunEarlyRunQueueInitialize(BolunEarlyRunQueue *queue);
BolunSchedulerStatus BolunEarlyRunQueuePush(BolunEarlyRunQueue *queue, BolunRunnableThread thread);
BolunSchedulerStatus BolunEarlyRunQueuePopNext(BolunEarlyRunQueue *queue, BolunRunnableThread *thread);

#ifdef __cplusplus
}
#endif

#endif
