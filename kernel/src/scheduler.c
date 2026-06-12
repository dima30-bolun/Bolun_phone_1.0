#include "bolun/kernel/scheduler.h"

static size_t BolunFindBestRunnableIndex(const BolunEarlyRunQueue *queue)
{
    size_t best_index = BOLUN_EARLY_RUN_QUEUE_CAPACITY;

    for (size_t index = 0; index < queue->length; ++index) {
        if (!queue->occupied[index]) {
            continue;
        }
        if (best_index == BOLUN_EARLY_RUN_QUEUE_CAPACITY) {
            best_index = index;
            continue;
        }

        const BolunRunnableThread *candidate = &queue->entries[index];
        const BolunRunnableThread *current = &queue->entries[best_index];
        if (candidate->priority > current->priority ||
            (candidate->priority == current->priority &&
             candidate->enqueued_at_nanoseconds < current->enqueued_at_nanoseconds)) {
            best_index = index;
        }
    }

    return best_index;
}

void BolunEarlyRunQueueInitialize(BolunEarlyRunQueue *queue)
{
    if (queue == NULL) {
        return;
    }

    queue->length = 0;
    for (size_t index = 0; index < BOLUN_EARLY_RUN_QUEUE_CAPACITY; ++index) {
        queue->occupied[index] = false;
        queue->entries[index] = (BolunRunnableThread){{0U}, 0U, 0U};
    }
}

BolunSchedulerStatus BolunEarlyRunQueuePush(BolunEarlyRunQueue *queue, BolunRunnableThread thread)
{
    if (queue == NULL || thread.thread_id.value == 0U) {
        return BOLUN_SCHEDULER_STATUS_BAD_ARGUMENT;
    }
    if (queue->length >= BOLUN_EARLY_RUN_QUEUE_CAPACITY) {
        return BOLUN_SCHEDULER_STATUS_QUEUE_FULL;
    }

    queue->entries[queue->length] = thread;
    queue->occupied[queue->length] = true;
    ++queue->length;
    return BOLUN_SCHEDULER_STATUS_OK;
}

BolunSchedulerStatus BolunEarlyRunQueuePopNext(BolunEarlyRunQueue *queue, BolunRunnableThread *thread)
{
    if (queue == NULL || thread == NULL) {
        return BOLUN_SCHEDULER_STATUS_BAD_ARGUMENT;
    }
    if (queue->length == 0U) {
        return BOLUN_SCHEDULER_STATUS_EMPTY;
    }

    const size_t selected_index = BolunFindBestRunnableIndex(queue);
    if (selected_index == BOLUN_EARLY_RUN_QUEUE_CAPACITY) {
        return BOLUN_SCHEDULER_STATUS_EMPTY;
    }

    *thread = queue->entries[selected_index];
    for (size_t index = selected_index; index + 1U < queue->length; ++index) {
        queue->entries[index] = queue->entries[index + 1U];
        queue->occupied[index] = queue->occupied[index + 1U];
    }
    --queue->length;
    queue->occupied[queue->length] = false;
    queue->entries[queue->length] = (BolunRunnableThread){{0U}, 0U, 0U};

    return BOLUN_SCHEDULER_STATUS_OK;
}
