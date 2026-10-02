#include "MinHeap.h"
#include <utility>
using namespace std;

static bool entryLess(const HeapEntry &a, const HeapEntry &b) {
    if (a.priority != b.priority) return a.priority < b.priority;
    if (a.arrival != b.arrival) return a.arrival < b.arrival;
    return a.pid < b.pid;
}

static void heapifyUp(MinHeap &h, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!entryLess(h.data[i], h.data[parent])) break;
        swap(h.data[parent], h.data[i]);
        i = parent;
    }
}

static void heapifyDown(MinHeap &h, int i) {
    int n = (int)h.data.size();
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < n && entryLess(h.data[left], h.data[smallest])) smallest = left;
        if (right < n && entryLess(h.data[right], h.data[smallest])) smallest = right;
        if (smallest == i) break;
        swap(h.data[i], h.data[smallest]);
        i = smallest;
    }
}

void pushMinHeap(MinHeap &h, int priority, int vectorIndex, int arrival, int pid) {
    HeapEntry e;
    e.priority = priority;
    e.vectorIndex = vectorIndex;
    e.arrival = arrival;
    e.pid = pid;
    h.data.push_back(e);
    heapifyUp(h, (int)h.data.size() - 1);
}

HeapEntry popMinHeap(MinHeap &h) {
    HeapEntry top = h.data.front();
    h.data[0] = h.data.back();
    h.data.pop_back();
    if (!h.data.empty()) heapifyDown(h, 0);
    return top;
}

bool isMinHeapEmpty(const MinHeap &h) {
    return h.data.empty();
}

int minHeapSize(const MinHeap &h) {
    return (int)h.data.size();
}

void clearMinHeap(MinHeap &h) {
    h.data.clear();
}
