#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <vector>
#include "Process.h"
#include "UndoStack.h"
#include "ProcessBST.h"
#include "DependencyGraph.h"
#include "HistoryList.h"
#include "Sorter.h"

// ProcessManager.h
// -----------------
// One struct that groups the process table with every supporting data
// structure it depends on, plus free functions that operate on it. Every
// menu feature goes through these functions so the structures stay
// consistent with each other.

struct ProcessManager {
    std::vector<Process> processes;
    UndoStack undo;
    ProcessBST bst;
    DependencyGraph graph;
    HistoryList history;
};

bool addProcessToManager(ProcessManager &m, int pid, int arrivalTime, int burstTime, int priority);
bool deleteProcessFromManager(ProcessManager &m, int pid);
bool updateProcessInManager(ProcessManager &m, int pid, int newArrivalTime, int newBurstTime, int newPriority);

void displayAllProcesses(const ProcessManager &m);
bool searchProcessBST(const ProcessManager &m, int pid);      // O(log n) average, via the BST
bool searchProcessBinary(const ProcessManager &m, int pid);   // O(log n), via Binary Search on a pid-sorted copy

void sortManagerProcesses(ProcessManager &m, SortField field, SortAlgo algo);

bool undoLastOperation(ProcessManager &m);

bool processExists(const ProcessManager &m, int pid);
int processCount(const ProcessManager &m);

void rebuildManagerBST(ProcessManager &m);

// Frees every heap-allocated node owned by the manager (undo stack, BST,
// history list). Call once before the program exits.
void clearManager(ProcessManager &m);

#endif
