#include "Process.h"
#include <iostream>
#include <iomanip>
using namespace std;

Process makeProcess(int pid, int arrivalTime, int burstTime, int priority) {
    Process p;
    p.pid = pid;
    p.arrivalTime = arrivalTime;
    p.burstTime = burstTime;
    p.priority = priority;
    p.completionTime = 0;
    p.waitingTime = 0;
    p.turnaroundTime = 0;
    p.remainingTime = burstTime;
    return p;
}

void resetMetrics(Process &p) {
    p.completionTime = 0;
    p.waitingTime = 0;
    p.turnaroundTime = 0;
    p.remainingTime = p.burstTime;
}

void printProcessHeader() {
    cout << left
         << setw(8)  << "PID"
         << setw(14) << "Arrival"
         << setw(12) << "Burst"
         << setw(10) << "Priority"
         << setw(14) << "Completion"
         << setw(12) << "Waiting"
         << setw(14) << "Turnaround"
         << "\n";
    cout << string(84, '-') << "\n";
}

void printProcess(const Process &p) {
    cout << left
         << setw(8)  << p.pid
         << setw(14) << p.arrivalTime
         << setw(12) << p.burstTime
         << setw(10) << p.priority
         << setw(14) << p.completionTime
         << setw(12) << p.waitingTime
         << setw(14) << p.turnaroundTime
         << "\n";
}
