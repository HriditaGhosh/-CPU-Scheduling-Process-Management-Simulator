#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include "Process.h"
#include "ReadyQueueList.h"
#include "CircularQueue.h"
#include "MinHeap.h"
#include "HistoryList.h"

// Scheduler.h
// ------------
// Free functions implementing the three CPU scheduling algorithms:
//   - FCFS         : driven by the singly-linked-list Ready Queue
//   - Round Robin   : driven by the array-based Circular Queue
//   - Priority       : driven by the Min Heap (priority queue)

struct GanttBlock {
    int pid = -1;   // -1 represents an idle CPU block
    int startTime = 0;
    int endTime = 0;
};

struct SchedulingResult {
    std::string algorithmName;
    std::vector<Process> processes;
    std::vector<GanttBlock> gantt;
    double avgWaitingTime = 0.0;
    double avgTurnaroundTime = 0.0;
};

SchedulingResult runFCFS(std::vector<Process> processes, HistoryList &history);
SchedulingResult runRoundRobin(std::vector<Process> processes, int quantum, HistoryList &history);
SchedulingResult runPriority(std::vector<Process> processes, HistoryList &history);

void printGanttChart(const std::vector<GanttBlock> &gantt);
void printSchedulingResult(const SchedulingResult &result);

#endif
