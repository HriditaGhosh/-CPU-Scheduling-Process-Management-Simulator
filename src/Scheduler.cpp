#include "Scheduler.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <iomanip>
#include <unordered_map>
using namespace std;

static void computeAverages(SchedulingResult &result) {
    if (result.processes.empty()) return;
    double totalWaiting = 0.0, totalTurnaround = 0.0;
    for (const auto &p : result.processes) {
        totalWaiting += p.waitingTime;
        totalTurnaround += p.turnaroundTime;
    }
    result.avgWaitingTime = totalWaiting / result.processes.size();
    result.avgTurnaroundTime = totalTurnaround / result.processes.size();
}

static bool byArrivalThenPid(const Process &a, const Process &b) {
    if (a.arrivalTime != b.arrivalTime) return a.arrivalTime < b.arrivalTime;
    return a.pid < b.pid;
}

// ------------------------------------------------------------------ FCFS --
SchedulingResult runFCFS(vector<Process> processes, HistoryList &history) {
    SchedulingResult result;
    result.algorithmName = "FCFS (First Come First Serve)";
    if (processes.empty()) return result;

    sort(processes.begin(), processes.end(), byArrivalThenPid);

    unordered_map<int, int> pidToIndex;
    ReadyQueueList readyQueue;
    for (int i = 0; i < (int)processes.size(); ++i) {
        pidToIndex[processes[i].pid] = i;
        enqueuePid(readyQueue, processes[i].pid);
    }

    int currentTime = 0;
    int pid;
    while (dequeuePid(readyQueue, pid)) {
        Process &p = processes[pidToIndex[pid]];
        if (p.arrivalTime > currentTime) {
            result.gantt.push_back({-1, currentTime, p.arrivalTime});
            currentTime = p.arrivalTime;
        }
        int start = currentTime;
        currentTime += p.burstTime;
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        result.gantt.push_back({p.pid, start, currentTime});
        insertCompletedProcess(history, p);
    }

    result.processes = processes;
    computeAverages(result);
    return result;
}

// ------------------------------------------------------------ Round Robin --
SchedulingResult runRoundRobin(vector<Process> processes, int quantum, HistoryList &history) {
    SchedulingResult result;
    result.algorithmName = "Round Robin (Quantum = " + to_string(quantum) + ")";
    if (processes.empty() || quantum <= 0) return result;

    sort(processes.begin(), processes.end(), byArrivalThenPid);
    int n = (int)processes.size();
    for (auto &p : processes) p.remainingTime = p.burstTime;

    CircularQueue cq;
    initCircularQueue(cq, n + 1);
    vector<bool> arrivedFlag(n, false);
    int completedCount = 0;
    int currentTime = 0;

    auto pushNewArrivals = [&](int uptoTime, int excludeIdx) {
        for (int i = 0; i < n; ++i) {
            if (i == excludeIdx) continue;
            if (!arrivedFlag[i] && processes[i].arrivalTime <= uptoTime) {
                enqueueCQ(cq, i);
                arrivedFlag[i] = true;
            }
        }
    };

    int minArrival = INT_MAX;
    for (auto &p : processes) minArrival = min(minArrival, p.arrivalTime);
    if (currentTime < minArrival) {
        result.gantt.push_back({-1, currentTime, minArrival});
        currentTime = minArrival;
    }
    pushNewArrivals(currentTime, -1);

    int idx;
    while (completedCount < n) {
        if (!dequeueCQ(cq, idx)) {
            int earliest = INT_MAX;
            for (int i = 0; i < n; ++i) if (!arrivedFlag[i]) earliest = min(earliest, processes[i].arrivalTime);
            if (earliest == INT_MAX) break;
            result.gantt.push_back({-1, currentTime, earliest});
            currentTime = earliest;
            pushNewArrivals(currentTime, -1);
            continue;
        }

        Process &p = processes[idx];
        int runFor = min(quantum, p.remainingTime);
        int start = currentTime;
        currentTime += runFor;
        p.remainingTime -= runFor;
        result.gantt.push_back({p.pid, start, currentTime});

        pushNewArrivals(currentTime, idx);

        if (p.remainingTime > 0) {
            enqueueCQ(cq, idx);
        } else {
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            completedCount++;
            insertCompletedProcess(history, p);
        }
    }

    destroyCircularQueue(cq);
    result.processes = processes;
    computeAverages(result);
    return result;
}

// -------------------------------------------------------------- Priority --
SchedulingResult runPriority(vector<Process> processes, HistoryList &history) {
    SchedulingResult result;
    result.algorithmName = "Priority Scheduling (Non-Preemptive)";
    if (processes.empty()) return result;

    sort(processes.begin(), processes.end(), byArrivalThenPid);
    int n = (int)processes.size();
    vector<bool> pushed(n, false);
    MinHeap heap;
    int currentTime = 0;
    int completedCount = 0;

    while (completedCount < n) {
        for (int i = 0; i < n; ++i) {
            if (!pushed[i] && processes[i].arrivalTime <= currentTime) {
                pushMinHeap(heap, processes[i].priority, i, processes[i].arrivalTime, processes[i].pid);
                pushed[i] = true;
            }
        }

        if (isMinHeapEmpty(heap)) {
            int earliest = INT_MAX;
            for (int i = 0; i < n; ++i) if (!pushed[i]) earliest = min(earliest, processes[i].arrivalTime);
            if (earliest == INT_MAX) break;
            result.gantt.push_back({-1, currentTime, earliest});
            currentTime = earliest;
            continue;
        }

        HeapEntry top = popMinHeap(heap);
        Process &p = processes[top.vectorIndex];
        int start = currentTime;
        currentTime += p.burstTime;
        p.completionTime = currentTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        completedCount++;
        result.gantt.push_back({p.pid, start, currentTime});
        insertCompletedProcess(history, p);
    }

    result.processes = processes;
    computeAverages(result);
    return result;
}

// ---------------------------------------------------------- Gantt chart --
void printGanttChart(const vector<GanttBlock> &gantt) {
    if (gantt.empty()) {
        cout << "(no schedule to display)\n";
        return;
    }

    const int colWidth = 9;
    cout << "\nGantt Chart:\n ";
    for (size_t i = 0; i < gantt.size(); ++i) cout << string(colWidth, '-') << " ";
    cout << "\n|";
    for (const auto &b : gantt) {
        string label = (b.pid == -1) ? "IDLE" : ("P" + to_string(b.pid));
        int pad = colWidth - (int)label.size();
        int left = pad / 2, right = pad - left;
        cout << string(left, ' ') << label << string(right, ' ') << "|";
    }
    cout << "\n ";
    for (size_t i = 0; i < gantt.size(); ++i) cout << string(colWidth, '-') << " ";
    cout << "\n";

    cout << right << gantt.front().startTime;
    for (const auto &b : gantt) cout << setw(colWidth + 1) << b.endTime;
    cout << left << "\n"; // restore the default alignment used by the table printers
}

// -------------------------------------------------------------- results --
void printSchedulingResult(const SchedulingResult &result) {
    cout << "\n=========== " << result.algorithmName << " ===========\n";
    if (result.processes.empty()) {
        cout << "No processes to schedule.\n";
        return;
    }

    vector<Process> byPid = result.processes;
    sort(byPid.begin(), byPid.end(), [](const Process &a, const Process &b) { return a.pid < b.pid; });

    printProcessHeader();
    for (const auto &p : byPid) printProcess(p);

    printGanttChart(result.gantt);

    ios::fmtflags savedFlags = cout.flags();
    streamsize savedPrecision = cout.precision();
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time    : " << result.avgWaitingTime << "\n";
    cout << "Average Turnaround Time : " << result.avgTurnaroundTime << "\n";
    cout.flags(savedFlags);
    cout.precision(savedPrecision);
}
