#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

// CircularQueue.h  (Circular Queue : Round Robin)
// -----------------------------------------------------
// Manual, array-based CIRCULAR QUEUE used to drive Round Robin scheduling.
// front/rear wrap around the fixed array using modulo arithmetic.

struct CircularQueue {
    int *arr = nullptr;
    int capacity = 0;
    int frontIdx = 0;
    int rearIdx = -1;
    int count = 0;
};

void initCircularQueue(CircularQueue &q, int capacity);
void destroyCircularQueue(CircularQueue &q);
bool isCQEmpty(const CircularQueue &q);
bool isCQFull(const CircularQueue &q);
bool enqueueCQ(CircularQueue &q, int value);
bool dequeueCQ(CircularQueue &q, int &value);
int cqSize(const CircularQueue &q);

#endif
