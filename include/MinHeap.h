#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>

// MinHeap.h  (Lab 10 : Min Heap)
// --------------------------------------
// Manual array-based MIN HEAP used as the priority queue that drives
// Priority Scheduling. Lower priority number = more urgent (pops first).

struct HeapEntry {
    int priority = 0;
    int vectorIndex = -1;
    int arrival = 0; // tie-break 1: earlier arrival first
    int pid = 0;     // tie-break 2: smaller PID first
};

struct MinHeap {
    std::vector<HeapEntry> data;
};

// Entries are ordered by (priority, arrival, pid) so equal priorities are
// served in a deterministic, fair order.
void pushMinHeap(MinHeap &h, int priority, int vectorIndex, int arrival = 0, int pid = 0);
HeapEntry popMinHeap(MinHeap &h); // caller must check isMinHeapEmpty() first
bool isMinHeapEmpty(const MinHeap &h);
int minHeapSize(const MinHeap &h);
void clearMinHeap(MinHeap &h);

#endif
