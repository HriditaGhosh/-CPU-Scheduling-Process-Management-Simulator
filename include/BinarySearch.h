#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <vector>
#include "Process.h"

// BinarySearch.h  (Lab 11 : Binary Search)
// ------------------------------------------------
// Manual iterative binary search over a vector of processes that is
// already sorted ascending by PID. Returns the index of the match, or -1.

int binarySearchByPid(const std::vector<Process> &sortedByPid, int pid);

#endif
