#ifndef PROCESS_H
#define PROCESS_H

// Process.h
// ----------
// Plain struct (no OOP encapsulation) holding one process record, plus
// free functions that operate on it. Shared by every data structure and
// algorithm in the project.

struct Process {
    int pid = 0;
    int arrivalTime = 0;
    int burstTime = 0;
    int priority = 0;

    // Fields filled in while a scheduling algorithm runs
    int completionTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
    int remainingTime = 0; // used by Round Robin
};

Process makeProcess(int pid, int arrivalTime, int burstTime, int priority);
void resetMetrics(Process &p);
void printProcessHeader();
void printProcess(const Process &p);

#endif
