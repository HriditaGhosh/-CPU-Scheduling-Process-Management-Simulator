#ifndef READY_QUEUE_LIST_H
#define READY_QUEUE_LIST_H

// ReadyQueueList.h  (Lab 2 : Queue, Lab 4 : Singly Linked List)
// -----------------------------------------------------------------
// Manual SINGLY LINKED LIST used as a dynamic Ready Queue (FIFO) for FCFS.
// No STL std::queue used -- enqueue/dequeue are hand-written pointer
// operations.

struct SLLNode {
    int pid = 0;
    SLLNode *next = nullptr;
};

struct ReadyQueueList {
    SLLNode *head = nullptr;
    SLLNode *tail = nullptr;
    int count = 0;
};

void enqueuePid(ReadyQueueList &q, int pid);
bool dequeuePid(ReadyQueueList &q, int &pid);
bool isQueueEmpty(const ReadyQueueList &q);
int queueSize(const ReadyQueueList &q);
void clearQueue(ReadyQueueList &q);
void printQueue(const ReadyQueueList &q);

#endif
