#include "ReadyQueueList.h"
#include <iostream>
using namespace std;

// enqueue: insert a new node at the tail.
void enqueuePid(ReadyQueueList &q, int pid) {
    SLLNode *node = new SLLNode();
    node->pid = pid;
    node->next = nullptr;
    if (q.tail == nullptr) {
        q.head = q.tail = node;
    } else {
        q.tail->next = node;
        q.tail = node;
    }
    q.count++;
}

// dequeue: remove the node at the head.
bool dequeuePid(ReadyQueueList &q, int &pid) {
    if (q.head == nullptr) return false;
    SLLNode *node = q.head;
    pid = node->pid;
    q.head = q.head->next;
    if (q.head == nullptr) q.tail = nullptr;
    delete node;
    q.count--;
    return true;
}

bool isQueueEmpty(const ReadyQueueList &q) {
    return q.head == nullptr;
}

int queueSize(const ReadyQueueList &q) {
    return q.count;
}

void clearQueue(ReadyQueueList &q) {
    while (q.head != nullptr) {
        SLLNode *node = q.head;
        q.head = q.head->next;
        delete node;
    }
    q.tail = nullptr;
    q.count = 0;
}

void printQueue(const ReadyQueueList &q) {
    if (q.head == nullptr) {
        cout << "(ready queue empty)\n";
        return;
    }
    SLLNode *cur = q.head;
    cout << "Front -> ";
    while (cur != nullptr) {
        cout << "P" << cur->pid;
        if (cur->next != nullptr) cout << " -> ";
        cur = cur->next;
    }
    cout << " <- Back\n";
}
