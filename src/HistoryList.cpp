#include "HistoryList.h"
#include <iostream>
using namespace std;

// insert at the tail (i.e. in completion order).
void insertCompletedProcess(HistoryList &h, Process p) {
    DLLNode *node = new DLLNode();
    node->data = p;
    node->prev = nullptr;
    node->next = nullptr;
    if (h.tail == nullptr) {
        h.head = h.tail = node;
    } else {
        node->prev = h.tail;
        h.tail->next = node;
        h.tail = node;
    }
    h.count++;
}

void traverseForwardHistory(const HistoryList &h) {
    if (h.head == nullptr) {
        cout << "(no completed processes yet)\n";
        return;
    }
    cout << "\n--- Completed History (Forward: oldest -> newest) ---\n";
    printProcessHeader();
    DLLNode *cur = h.head;
    while (cur != nullptr) {
        printProcess(cur->data);
        cur = cur->next;
    }
}

void traverseBackwardHistory(const HistoryList &h) {
    if (h.tail == nullptr) {
        cout << "(no completed processes yet)\n";
        return;
    }
    cout << "\n--- Completed History (Backward: newest -> oldest) ---\n";
    printProcessHeader();
    DLLNode *cur = h.tail;
    while (cur != nullptr) {
        printProcess(cur->data);
        cur = cur->prev;
    }
}

int historySize(const HistoryList &h) {
    return h.count;
}

void clearHistory(HistoryList &h) {
    while (h.head != nullptr) {
        DLLNode *node = h.head;
        h.head = h.head->next;
        delete node;
    }
    h.tail = nullptr;
    h.count = 0;
}
