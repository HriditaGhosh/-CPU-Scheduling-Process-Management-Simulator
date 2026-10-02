#ifndef SORTER_H
#define SORTER_H

#include <vector>
#include "Process.h"

// Sorter.h  (Lab 7 : Bubble/Selection/Insertion/Merge/Quick, Lab 10 : Heap Sort)
// ----------------------------------------------------------------------------------
// Plain functions (no class) that sort the process table by any field using
// any of six classic algorithms.

enum SortField { FIELD_PID, FIELD_ARRIVAL, FIELD_BURST, FIELD_PRIORITY };
enum SortAlgo  { ALGO_BUBBLE, ALGO_SELECTION, ALGO_INSERTION, ALGO_MERGE, ALGO_QUICK, ALGO_HEAP };

void sortProcesses(std::vector<Process> &v, SortField field, SortAlgo algo);
const char *sortFieldName(SortField field);
const char *sortAlgoName(SortAlgo algo);

#endif
