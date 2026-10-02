#ifndef HISTORY_LIST_H
#define HISTORY_LIST_H

#include "Process.h"

// HistoryList.h  (Lab 5 / 6 : Doubly Linked List)
// --------------------------------------------------
// Manual DOUBLY LINKED LIST storing completed processes, so the history can
// be walked forward (oldest -> newest) and backward (newest -> oldest).

struct DLLNode {
    Process data;
    DLLNode *prev = nullptr;
    DLLNode *next = nullptr;
};

struct HistoryList {
    DLLNode *head = nullptr;
    DLLNode *tail = nullptr;
    int count = 0;
};

void insertCompletedProcess(HistoryList &h, Process p);
void traverseForwardHistory(const HistoryList &h);
void traverseBackwardHistory(const HistoryList &h);
int historySize(const HistoryList &h);
void clearHistory(HistoryList &h);

#endif
