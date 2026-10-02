// main.cpp
// ---------
// DSA Workbench: CPU Scheduling & Process Management Simulator
//
// Plain, procedural console application (structs + free functions, no
// classes/encapsulation). Every data structure (vector, stack, queue,
// circular queue, singly linked list, doubly linked list, BST, min heap,
// graph) and algorithm (6 sorts, binary search, FCFS, Round Robin, Priority,
// BFS, DFS, Topological Sort) is used inside the feature that needs it.

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

#include "ProcessManager.h"
#include "Sorter.h"
#include "Scheduler.h"
#include "DependencyGraph.h"

using namespace std;

// ---------------------------------------------------------------- input --
// Invariant kept by every read helper below: once the function returns, the
// input stream is positioned right after that line's newline, with nothing
// left over. That way pause() never has to guess how the previous read was
// done (cin >> vs getline) -- it just waits for one more Enter press.
static int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard rest of the line
            return value;
        }
        if (cin.eof()) {
            cout << "\nNo more input. Exiting.\n";
            exit(0);
        }
        cout << "Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static void pause() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// --------------------------------------------------------------- menus --
static void printMainMenu() {
    cout << "\n==================================================================\n";
    cout << "   DSA WORKBENCH : CPU Scheduling & Process Management Simulator\n";
    cout << "==================================================================\n";
    cout << " 1.  Add Process\n";
    cout << " 2.  Delete Process\n";
    cout << " 3.  Update Process\n";
    cout << " 4.  Display Processes\n";
    cout << " 5.  Search Process (BST or Binary Search)\n";
    cout << " 6.  Sort Processes\n";
    cout << " 7.  Undo Last Operation\n";
    cout << " 8.  Process Dependencies (Graph: BFS / DFS / Topological Sort)\n";
    cout << " 9.  Run a Scheduling Algorithm\n";
    cout << "10.  Compare All Scheduling Algorithms\n";
    cout << "11.  View Completed Process History\n";
    cout << " 0.  Exit\n";
    cout << "------------------------------------------------------------------\n";
}

// --------------------------------------------------------------- add --
static void handleAddProcess(ProcessManager &manager) {
    cout << "\n-- Add Process --\n";
    int pid = readInt("Enter Process ID: ");
    if (processExists(manager, pid)) {
        cout << "A process with PID " << pid << " already exists.\n";
        return;
    }
    int arrival = readInt("Enter Arrival Time: ");
    if (arrival < 0) {
        cout << "Arrival Time cannot be negative. Process not added.\n";
        return;
    }
    int burst = readInt("Enter Burst Time: ");
    if (burst <= 0) {
        cout << "Burst Time must be positive. Process not added.\n";
        return;
    }
    int priority = readInt("Enter Priority (lower number = higher priority): ");

    addProcessToManager(manager, pid, arrival, burst, priority);
    cout << "Process P" << pid << " added successfully.\n";
}

// --------------------------------------------------------------- delete --
static void handleDeleteProcess(ProcessManager &manager) {
    cout << "\n-- Delete Process --\n";
    int pid = readInt("Enter Process ID to delete: ");
    if (deleteProcessFromManager(manager, pid)) cout << "Process P" << pid << " deleted successfully.\n";
    else cout << "Process P" << pid << " not found.\n";
}

// --------------------------------------------------------------- update --
static void handleUpdateProcess(ProcessManager &manager) {
    cout << "\n-- Update Process --\n";
    int pid = readInt("Enter Process ID to update: ");
    if (!processExists(manager, pid)) {
        cout << "Process P" << pid << " not found.\n";
        return;
    }
    int arrival = readInt("Enter new Arrival Time: ");
    if (arrival < 0) {
        cout << "Arrival Time cannot be negative. Update cancelled.\n";
        return;
    }
    int burst = readInt("Enter new Burst Time: ");
    if (burst <= 0) {
        cout << "Burst Time must be positive. Update cancelled.\n";
        return;
    }
    int priority = readInt("Enter new Priority: ");

    updateProcessInManager(manager, pid, arrival, burst, priority);
    cout << "Process P" << pid << " updated successfully.\n";
}

// --------------------------------------------------------------- search --
static void handleSearchProcess(const ProcessManager &manager) {
    cout << "\n-- Search Process --\n";
    cout << " 1. Binary Search Tree lookup\n";
    cout << " 2. Binary Search (on a PID-sorted array)\n";
    int method = readInt("Choice: ");
    if (method != 1 && method != 2) { cout << "Invalid choice.\n"; return; }
    int pid = readInt("Enter Process ID to search: ");

    bool found = (method == 1) ? searchProcessBST(manager, pid)
                               : searchProcessBinary(manager, pid);

    if (!found) cout << "Process P" << pid << " not found.\n";
}

// ----------------------------------------------------------------- sort --
static void handleSortProcesses(ProcessManager &manager) {
    if (processCount(manager) == 0) {
        cout << "No processes to sort.\n";
        return;
    }
    cout << "\n-- Sort Processes --\n";
    cout << "Sort by field:\n";
    cout << " 1. Process ID\n 2. Arrival Time\n 3. Burst Time\n 4. Priority\n";
    int fieldChoice = readInt("Choice: ");

    SortField field;
    switch (fieldChoice) {
        case 1: field = FIELD_PID; break;
        case 2: field = FIELD_ARRIVAL; break;
        case 3: field = FIELD_BURST; break;
        case 4: field = FIELD_PRIORITY; break;
        default: cout << "Invalid choice.\n"; return;
    }

    cout << "Sort using algorithm:\n";
    cout << " 1. Bubble Sort\n 2. Selection Sort\n 3. Insertion Sort\n 4. Merge Sort\n 5. Quick Sort\n 6. Heap Sort\n";
    int algoChoice = readInt("Choice: ");

    SortAlgo algo;
    switch (algoChoice) {
        case 1: algo = ALGO_BUBBLE; break;
        case 2: algo = ALGO_SELECTION; break;
        case 3: algo = ALGO_INSERTION; break;
        case 4: algo = ALGO_MERGE; break;
        case 5: algo = ALGO_QUICK; break;
        case 6: algo = ALGO_HEAP; break;
        default: cout << "Invalid choice.\n"; return;
    }

    sortManagerProcesses(manager, field, algo);
    cout << "\nProcesses sorted by " << sortFieldName(field) << " using " << sortAlgoName(algo) << ".\n";
    displayAllProcesses(manager);
}

// ----------------------------------------------------------------- undo --
static void handleUndo(ProcessManager &manager) {
    if (undoLastOperation(manager)) {
        cout << "Last operation undone successfully.\n";
        displayAllProcesses(manager);
    } else {
        cout << "Nothing to undo.\n";
    }
}

// ------------------------------------------------------------- graph -- --
static void handleDependencies(ProcessManager &manager) {
    cout << "\n-- Process Dependencies (Directed Graph) --\n";
    cout << " 1. Add Dependency (Process A must finish before Process B)\n";
    cout << " 2. View Adjacency List\n";
    cout << " 3. BFS Traversal from a Process\n";
    cout << " 4. DFS Traversal from a Process\n";
    cout << " 5. Topological Sort (valid execution order)\n";
    cout << " 0. Back\n";
    int choice = readInt("Choice: ");

    DependencyGraph &graph = manager.graph;

    switch (choice) {
        case 1: {
            int a = readInt("Enter PID that must run FIRST: ");
            int b = readInt("Enter PID that depends on it (runs AFTER): ");
            if (!processExists(manager, a) || !processExists(manager, b)) {
                cout << "Both processes must already exist.\n";
            } else if (addDependency(graph, a, b)) {
                cout << "Dependency added: P" << a << " -> P" << b << "\n";
            } else {
                cout << "Could not add that dependency (check the PIDs).\n";
            }
            break;
        }
        case 2:
            printAdjList(graph);
            break;
        case 3: {
            int start = readInt("Start BFS from PID: ");
            vector<int> order = bfsTraversal(graph, start);
            if (order.empty()) { cout << "PID not found in graph.\n"; break; }
            cout << "BFS order: ";
            for (size_t i = 0; i < order.size(); ++i) cout << "P" << order[i] << (i + 1 < order.size() ? " -> " : "\n");
            break;
        }
        case 4: {
            int start = readInt("Start DFS from PID: ");
            vector<int> order = dfsTraversal(graph, start);
            if (order.empty()) { cout << "PID not found in graph.\n"; break; }
            cout << "DFS order: ";
            for (size_t i = 0; i < order.size(); ++i) cout << "P" << order[i] << (i + 1 < order.size() ? " -> " : "\n");
            break;
        }
        case 5: {
            vector<int> order;
            if (topoSort(graph, order)) {
                cout << "Valid execution order (Topological Sort): ";
                for (size_t i = 0; i < order.size(); ++i) cout << "P" << order[i] << (i + 1 < order.size() ? " -> " : "\n");
            } else {
                cout << "Cycle detected! These dependencies cannot be satisfied.\n";
            }
            break;
        }
        default:
            break;
    }
}

// --------------------------------------------------------- run scheduler --
static void handleRunScheduler(ProcessManager &manager) {
    if (processCount(manager) == 0) {
        cout << "No processes to schedule.\n";
        return;
    }
    cout << "\n-- Run a Scheduling Algorithm --\n";
    cout << " 1. FCFS (First Come First Serve)\n";
    cout << " 2. Round Robin\n";
    cout << " 3. Priority Scheduling\n";
    int choice = readInt("Choice: ");

    SchedulingResult result;
    // The history list shows the processes completed by the MOST RECENT run,
    // so each case drops the previous run's entries instead of piling up
    // duplicates.
    switch (choice) {
        case 1:
            clearHistory(manager.history);
            result = runFCFS(manager.processes, manager.history);
            break;
        case 2: {
            int quantum = readInt("Enter Time Quantum: ");
            if (quantum <= 0) { cout << "Quantum must be positive.\n"; return; }
            clearHistory(manager.history);
            result = runRoundRobin(manager.processes, quantum, manager.history);
            break;
        }
        case 3:
            clearHistory(manager.history);
            result = runPriority(manager.processes, manager.history);
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }
    printSchedulingResult(result);
}

// ------------------------------------------------------------- compare --
static void handleCompare(ProcessManager &manager) {
    if (processCount(manager) == 0) {
        cout << "No processes to schedule.\n";
        return;
    }
    int quantum = readInt("Enter Time Quantum to use for Round Robin in the comparison: ");
    if (quantum <= 0) { cout << "Quantum must be positive.\n"; return; }

    HistoryList scratchHistory; // kept separate from the manager's own history

    SchedulingResult fcfs = runFCFS(manager.processes, scratchHistory);
    SchedulingResult rr = runRoundRobin(manager.processes, quantum, scratchHistory);
    SchedulingResult priority = runPriority(manager.processes, scratchHistory);

    printSchedulingResult(fcfs);
    printSchedulingResult(rr);
    printSchedulingResult(priority);

    cout << "\n================= Comparison Summary =================\n";
    cout << left << setw(40) << "Algorithm" << setw(20) << "Avg Waiting Time" << "Avg Turnaround Time" << "\n";
    cout << string(80, '-') << "\n";

    const SchedulingResult *results[3] = {&fcfs, &rr, &priority};
    const SchedulingResult *best = results[0];
    for (int i = 0; i < 3; ++i) {
        cout << left << setw(40) << results[i]->algorithmName
             << setw(20) << fixed << setprecision(2) << results[i]->avgWaitingTime
             << results[i]->avgTurnaroundTime << "\n";
        if (results[i]->avgWaitingTime < best->avgWaitingTime) best = results[i];
    }

    cout << "\nRecommendation: \"" << best->algorithmName
         << "\" gives the lowest Average Waiting Time (" << fixed << setprecision(2) << best->avgWaitingTime
         << ") for this process set, so it is the best choice here.\n";

    // scratchHistory is local to this function and, unlike manager.history,
    // is never meant to persist -- free its nodes now instead of letting
    // them leak when scratchHistory goes out of scope.
    clearHistory(scratchHistory);
}

// ------------------------------------------------------------- history --
static void handleHistory(ProcessManager &manager) {
    if (historySize(manager.history) == 0) {
        cout << "No completed processes yet. Run a scheduling algorithm first.\n";
        return;
    }
    cout << "\n-- Completed Process History (Doubly Linked List) --\n";
    cout << " 1. Traverse Forward (oldest -> newest)\n";
    cout << " 2. Traverse Backward (newest -> oldest)\n";
    int choice = readInt("Choice: ");
    if (choice == 1) traverseForwardHistory(manager.history);
    else if (choice == 2) traverseBackwardHistory(manager.history);
    else cout << "Invalid choice.\n";
}

// ------------------------------------------------------------------ main --
int main() {
    ProcessManager manager;

    cout << "Welcome to the DSA Workbench: CPU Scheduling & Process Management Simulator\n";

    bool running = true;
    while (running) {
        printMainMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: handleAddProcess(manager); break;
            case 2: handleDeleteProcess(manager); break;
            case 3: handleUpdateProcess(manager); break;
            case 4: displayAllProcesses(manager); break;
            case 5: handleSearchProcess(manager); break;
            case 6: handleSortProcesses(manager); break;
            case 7: handleUndo(manager); break;
            case 8: handleDependencies(manager); break;
            case 9: handleRunScheduler(manager); break;
            case 10: handleCompare(manager); break;
            case 11: handleHistory(manager); break;
            case 0:
                cout << "Exiting DSA Workbench. Goodbye!\n";
                running = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        if (running) pause();
    }

    clearManager(manager); // free undo stack, BST and history nodes
    return 0;
}
