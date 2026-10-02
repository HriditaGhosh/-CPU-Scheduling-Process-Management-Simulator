#include "ProcessManager.h"
#include "BinarySearch.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Inserts entries[lo..hi] (already sorted by pid) middle-first, which yields a
// perfectly balanced BST. Recursion depth is only O(log n).
static void insertBalanced(ProcessBST &t, const vector<pair<int, int>> &entries, int lo, int hi) {
    if (lo > hi) return;
    int mid = lo + (hi - lo) / 2;
    insertBST(t, entries[mid].first, entries[mid].second);
    insertBalanced(t, entries, lo, mid - 1);
    insertBalanced(t, entries, mid + 1, hi);
}

// Rebuilds the pid -> vector-index BST from scratch. Inserting pids in table
// order would build a linked-list-shaped tree whenever the table is sorted by
// PID (or PIDs were added in increasing order), so the pids are sorted first
// and inserted median-first to keep the tree balanced (height ~ log2 n).
void rebuildManagerBST(ProcessManager &m) {
    clearBST(m.bst);
    vector<pair<int, int>> entries; // (pid, vector index)
    entries.reserve(m.processes.size());
    for (int i = 0; i < (int)m.processes.size(); ++i) entries.push_back({m.processes[i].pid, i});
    sort(entries.begin(), entries.end());
    insertBalanced(m.bst, entries, 0, (int)entries.size() - 1);
}

void clearManager(ProcessManager &m) {
    clearUndo(m.undo);
    clearBST(m.bst);
    clearHistory(m.history);
    clearGraph(m.graph);
    m.processes.clear();
}

bool processExists(const ProcessManager &m, int pid) {
    return searchBST(m.bst, pid) != -1;
}

int processCount(const ProcessManager &m) {
    return (int)m.processes.size();
}

bool addProcessToManager(ProcessManager &m, int pid, int arrivalTime, int burstTime, int priority) {
    if (processExists(m, pid)) return false; // PIDs must be unique

    Process p = makeProcess(pid, arrivalTime, burstTime, priority);
    m.processes.push_back(p);
    rebuildManagerBST(m); // keeps the BST balanced even for ascending PIDs
    addNode(m.graph, pid);

    UndoAction action;
    action.type = ACTION_ADD;
    action.afterState = p;
    pushUndo(m.undo, action);
    return true;
}

bool deleteProcessFromManager(ProcessManager &m, int pid) {
    int idx = searchBST(m.bst, pid);
    if (idx == -1) return false;

    Process removed = m.processes[idx];
    m.processes.erase(m.processes.begin() + idx);
    rebuildManagerBST(m);

    // removeNode() below permanently drops every edge touching pid, so
    // back them up first: this process's own dependents (outgoing) and
    // every other process that listed this one as a prerequisite
    // (incoming). Undo replays both to fully restore the graph.
    UndoAction action;
    action.type = ACTION_DELETE;
    action.beforeState = removed;
    action.index = idx; // so Undo can restore the same position

    auto outIt = m.graph.adjList.find(pid);
    if (outIt != m.graph.adjList.end()) action.outgoingEdges = outIt->second;
    for (const auto &entry : m.graph.adjList) {
        if (entry.first == pid) continue;
        for (int dependent : entry.second) {
            if (dependent == pid) action.incomingEdges.push_back(entry.first);
        }
    }

    removeNode(m.graph, pid);

    pushUndo(m.undo, action);
    return true;
}

bool updateProcessInManager(ProcessManager &m, int pid, int newArrivalTime, int newBurstTime, int newPriority) {
    int idx = searchBST(m.bst, pid);
    if (idx == -1) return false;

    Process before = m.processes[idx];
    m.processes[idx].arrivalTime = newArrivalTime;
    m.processes[idx].burstTime = newBurstTime;
    m.processes[idx].priority = newPriority;
    resetMetrics(m.processes[idx]);

    UndoAction action;
    action.type = ACTION_UPDATE;
    action.beforeState = before;
    action.afterState = m.processes[idx];
    pushUndo(m.undo, action);
    return true;
}

void displayAllProcesses(const ProcessManager &m) {
    if (m.processes.empty()) {
        cout << "No processes in the system yet.\n";
        return;
    }
    cout << "\n--- Current Process Table (" << m.processes.size() << " processes) ---\n";
    printProcessHeader();
    for (const auto &p : m.processes) printProcess(p);
}

bool searchProcessBST(const ProcessManager &m, int pid) {
    int idx = searchBST(m.bst, pid);
    if (idx == -1) return false;
    printProcessHeader();
    printProcess(m.processes[idx]);
    return true;
}

bool searchProcessBinary(const ProcessManager &m, int pid) {
    // Binary search needs a sorted-by-pid array, so we sort a scratch copy
    // (the manager's own stored order is left untouched).
    vector<Process> sortedCopy = m.processes;
    sortProcesses(sortedCopy, FIELD_PID, ALGO_QUICK);

    int idx = binarySearchByPid(sortedCopy, pid);
    if (idx == -1) return false;
    printProcessHeader();
    printProcess(sortedCopy[idx]);
    return true;
}

void sortManagerProcesses(ProcessManager &m, SortField field, SortAlgo algo) {
    sortProcesses(m.processes, field, algo);
    rebuildManagerBST(m); // vector order changed, so pid -> index mapping must be refreshed
}

bool undoLastOperation(ProcessManager &m) {
    UndoAction action;
    if (!popUndo(m.undo, action)) return false;

    switch (action.type) {
        case ACTION_ADD: {
            int idx = searchBST(m.bst, action.afterState.pid);
            if (idx != -1) {
                m.processes.erase(m.processes.begin() + idx);
                rebuildManagerBST(m);
                removeNode(m.graph, action.afterState.pid);
            }
            break;
        }
        case ACTION_DELETE: {
            int pos = action.index;
            if (pos < 0 || pos > (int)m.processes.size()) pos = (int)m.processes.size();
            m.processes.insert(m.processes.begin() + pos, action.beforeState);
            rebuildManagerBST(m);
            addNode(m.graph, action.beforeState.pid);
            for (int dependent : action.outgoingEdges) {
                addDependency(m.graph, action.beforeState.pid, dependent);
            }
            for (int prerequisite : action.incomingEdges) {
                addDependency(m.graph, prerequisite, action.beforeState.pid);
            }
            break;
        }
        case ACTION_UPDATE: {
            int idx = searchBST(m.bst, action.beforeState.pid);
            if (idx != -1) {
                m.processes[idx].arrivalTime = action.beforeState.arrivalTime;
                m.processes[idx].burstTime = action.beforeState.burstTime;
                m.processes[idx].priority = action.beforeState.priority;
                resetMetrics(m.processes[idx]);
            }
            break;
        }
    }
    return true;
}
