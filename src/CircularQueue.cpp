#include "CircularQueue.h"

void initCircularQueue(CircularQueue &q, int capacity) {
    q.arr = new int[capacity];
    q.capacity = capacity;
    q.frontIdx = 0;
    q.rearIdx = -1;
    q.count = 0;
}

void destroyCircularQueue(CircularQueue &q) {
    delete[] q.arr;
    q.arr = nullptr;
}

bool isCQEmpty(const CircularQueue &q) {
    return q.count == 0;
}

bool isCQFull(const CircularQueue &q) {
    return q.count == q.capacity;
}

bool enqueueCQ(CircularQueue &q, int value) {
    if (isCQFull(q)) return false;
    q.rearIdx = (q.rearIdx + 1) % q.capacity; // wrap around
    q.arr[q.rearIdx] = value;
    q.count++;
    return true;
}

bool dequeueCQ(CircularQueue &q, int &value) {
    if (isCQEmpty(q)) return false;
    value = q.arr[q.frontIdx];
    q.frontIdx = (q.frontIdx + 1) % q.capacity; // wrap around
    q.count--;
    return true;
}

int cqSize(const CircularQueue &q) {
    return q.count;
}
